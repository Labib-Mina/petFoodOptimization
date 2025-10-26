#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm> // Required for std::transform

// Include headers for the libraries
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

// Use the nlohmann::json namespace for convenience
using json = nlohmann::json;

// Function to get the current date in YYYYMMDD format
std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d");
    return ss.str();
}

// Function to sanitize brand name for the filename
std::string sanitizeBrand(std::string brand) {
    for (char &c : brand) {
        if (c == ' ') {
            c = '_';
        }
    }
    // Convert to lowercase
    std::transform(brand.begin(), brand.end(), brand.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return brand;
}


int main() {
    // 1. Set up your API Key and query
    // IMPORTANT: In a real app, load this from an environment variable or a secure config file.
    // Do not hardcode your API key in the source code.
    const char* apiKey = std::getenv("API_KEY");
    if (apiKey == nullptr) {
        std::cerr << "Error: API_KEY environment variable is not set." << std::endl;
        return 1;
    }

    std::string brandQuery;
    std::cout << "Enter a cat food brand (e.g., Tiki Cat): ";
    std::getline(std::cin, brandQuery);

    // 2. Construct the Gemini API prompt (same as your TypeScript file)
    std::string prompt = R"delimiter(
    Act as an agent with web browsing enabled to perform a two-part task for the brand: ")delimiter" + brandQuery + R"delimiter(".

    **Part A — Search & Collect**
    1.  **Normalize Brand:** Normalize the brand query (e.g., "tikki cat" -> "Tiki Cat").
    2.  **Find Products:** Search the web for the brand’s official site and major retailers (like Chewy, Petco) to discover products.
    3.  **Rank Popularity:** Use a heuristic combining retailer bestseller status, review counts, and visibility across reputable sources to find the top 5 most popular, mainstream products.
    4.  **Extract Data:** For each of the 5 products, extract: Product name, a direct URL to a clear product image, and nutrition facts (Protein, Fat/Oil, Fiber) with their source URL.
    5.  **Normalize & Filter (CRITICAL - MANDATORY):**
        *   Your primary goal is to report Protein, Fat, and Fiber in "g/100g".
        *   Convert percentages to g/100g (e.g., "Crude Protein 12%" on an as-fed basis directly translates to 12 g/100g. Do the same for Fat and Fiber percentages).
        *   **FILTERING REQUIREMENT:** If you cannot find a clear, numerical value for fiber for a specific product that can be confidently normalized to g/100g, **you must discard that product entirely and find a different popular product to take its place.**
        *   Your final list of 5 products must *only* contain items with complete and normalized nutritional information for Protein, Fat, and Fiber. The value for `fiber_g_per_100g` in the JSON output must always be a number, not `null`.

    **Output Format**
    You must return a single, valid JSON object inside a ```json code block. Do not include any other text, explanations, or markdown formatting outside of the JSON block. The JSON object must follow this exact structure:
    {
      "brand": "<Normalized brand>",
      "queried_at": "<ISO8601 timestamp>",
      "products": [
        {
          "name": "<Product name>",
          "image_url": "<Direct image URL>",
          "source_url": "<Nutrition source URL>",
          "serving_basis": "<e.g., per can 85 g, per 100 g>",
          "raw": { "protein": "<value as found>", "fat": "<value as found>", "fiber": "<value as found>" },
          "normalized": {
            "protein_g_per_100g": <number or null>,
            "fat_g_per_100g": <number or null>,
            "fiber_g_per_100g": <number>
          },
          "notes": "<any caveats: conversions used, data sources>"
        }
      ],
      "ranking_method": "bestseller + review_count + cross-source presence",
      "normalization_notes": "Describe formulas and any assumptions made for unit conversion."
    }
    )delimiter";

    // 3. Create the JSON request body
    json requestBody = {
        {"contents", {{
            {"parts", {{
                {"text", prompt}
            }}}
        }}},
        {"tools", {{
            {"googleSearch", json::object()}
        }}}
    };

    // 4. Make the HTTP request using CPR
    std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key=" + std::string(apiKey);
    
    std::cout << "Sending request to Gemini API..." << std::endl;

    cpr::Response r = cpr::Post(cpr::Url{url},
                                cpr::Body{requestBody.dump()},
                                cpr::Header{{"Content-Type", "application/json"}});

    if (r.status_code != 200) {
        std::cerr << "Error: API request failed with status code " << r.status_code << std::endl;
        std::cerr << "Response: " << r.text << std::endl;
        return 1;
    }
    
    std::cout << "API request successful. Parsing response..." << std::endl;

    try {
        // 5. Parse the outer JSON response from the API
        json apiResponse = json::parse(r.text);
        std::string modelResponseText = apiResponse["candidates"][0]["content"]["parts"][0]["text"];

        // Extract the JSON content from the markdown block
        std::string jsonContentString;
        size_t start = modelResponseText.find("```json");
        if (start != std::string::npos) {
            start += 7; // Skip ```json
            size_t end = modelResponseText.find("```", start);
            jsonContentString = modelResponseText.substr(start, end - start);
        } else {
            throw std::runtime_error("Could not find JSON content within a ```json block.");
        }

        // 6. Parse the inner JSON containing the cat food data
        json catFoodData = json::parse(jsonContentString);

        // 7. Format the data for the .txt file
// 7. Format the data for the .txt file
        std::stringstream tsvContent;
        // Add the header row first
        tsvContent << "product name\tprotein\tfats and oils\tfiber\n";

        for (const auto& product : catFoodData["products"]) {
            // Filter out products with null fiber, as a safeguard
            if (product["normalized"]["fiber_g_per_100g"].is_null()) {
                continue;
            }
            tsvContent << product["name"].get<std::string>() << "\t"
                       << product["normalized"]["protein_g_per_100g"].get<double>() << "\t"
                       << product["normalized"]["fat_g_per_100g"].get<double>() << "\t"
                       << product["normalized"]["fiber_g_per_100g"].get<double>() << "\n";
        }
        
        // 8. Write to a local .txt file
        std::string brandName = sanitizeBrand(catFoodData["brand"].get<std::string>());
        std::string filename = "cat_food_nutrients_" + brandName + "_" + getCurrentDate() + ".txt";

        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
            return 1;
        }

        outFile << tsvContent.str();
        outFile.close();

        std::cout << "\nSuccessfully saved nutritional data to: " << filename << std::endl;

    } catch (const json::parse_error& e) {
        std::cerr << "Error: Failed to parse JSON response." << std::endl;
        std::cerr << "Message: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}