#include "parser.hpp"
#include "API.hpp"
#include "CLI11/CLI11.h"
#include "MsgSystem/error.hpp"
#include "common.hpp"
#include "tinyxml2/tinyxml2.h"
#include <iostream>

using namespace ssmath3;

namespace fs = std::filesystem;

struct ImageFormatValidator : public CLI::Validator {
  ImageFormatValidator() {
    name_ = "IMAGE";
    func_ = [](const std::string &str) {
      std::string lower = CLI::detail::to_lower(str);

      // if ends with .png or .ppm
      if (lower.size() < 4 || (lower.substr(lower.size() - 4) != ".png" &&
                               lower.substr(lower.size() - 4) != ".ppm"))
        return std::string("File must have .png or .ppm extension");

      return std::string{};
    };
  }
};

struct XMLValidator : public CLI::Validator {
  XMLValidator() {
    name_ = "XML";
    func_ = [](const std::string &str) {
      std::string lower = CLI::detail::to_lower(str);

      // if ends with .xml
      if (lower.size() < 4 || (lower.substr(lower.size() - 4) != ".xml"))
        return std::string("File must have .xml extension");

      return std::string{};
    };
  }
};

const static ImageFormatValidator ImageFormatValidator;
const static XMLValidator XMLValidator;

namespace ssrt {

void Parser::validate_arguments(int argc, char **argv,
                                RunningOptions &run_opt) {
  CLI::App opts;
  opts.set_help_flag("--help,-h", "Print this help text.");

  std::string input_scene;

  opts.add_option("<input_scene_file>", input_scene, "")
      ->required()
      ->type_name("")
      ->check(CLI::ExistingFile)
      ->check(XMLValidator);

  std::string outfile;

  opts.add_option("--outfile,-o", outfile,
                  "Write the rendered image to <filename>.")
      ->type_name("<filename>")
      ->check(ImageFormatValidator);

  try {
    opts.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    std::exit(opts.exit(e));
  }

  run_opt.scene = input_scene;
  run_opt.outfile = outfile;

  opts.get_formatter()->column_width(40);
}

/// Generic convertion function.
/*!
 * This function receives a string,`attr_content`, that may contain one or more
 * instances of type `T`, and tries to convert this string into a list of actual
 * elements of type `T`.
 *
 * In case there is only one instance of T, that instance is stored in the
 * ParamSet object `ps`, passed as output parameter. If there are more than one
 * instance of T, the function extracts them into a `std::vector<T>` and stores
 * the vector in `ps`.
 *
 * Therefore, the output of this function is the ParamSet object `ps` that
 * contains either a single instance of T or a vector<T> with multiples
 * instances of T, found in `attr_content`.
 *
 * Example: convert("h_res", "1920", ps), here the attribute "h_res" should
 * become an integer. The function inserts ps->m_map["h_res"]=1920 (`int`), in
 * the ParamSet object `ps`.
 *
 * @param attr_name The attribute name that will be associated with the value in
 * the ParamSet.
 * @param attr_content The string attribute value (or values) we wish to
 * convert.
 * @param[out] ps The output ParamSet object (dictionary).
 *
 * @return true if convertion worked, false otherwise.
 */

template <typename T>
bool convert(const std::string &attr_name, const std::string &attr_content,
             ParamSet *ps) {
  // assert(ps);
  std::istringstream iss{
      attr_content}; // Make `attr_content` a stream to extract data from.
  T single_value{};  // Stores a single value of type T...
  std::vector<T>
      multiple_values; // ... or use this to try to store multiple values.
  bool input_string_still_has_values{
      true}; // Assume we have values to read from.
  // [1]: Try to read several T-values from the input string `attr_content`.
  while (input_string_still_has_values && !iss.eof()) {

    if constexpr (std::is_same_v<T, bool>) // Slightly different treatment if T
                                           // is bool
      iss >> std::boolalpha >>
          single_value; // Use std::boolalpha to read "true" ou "false"
    else
      iss >> single_value; // Regular extraction.

    // Failed while trying to extract value this time?
    if (iss.fail()) {
      if (multiple_values.empty()) // Is it completely empty?
        return false;              // Failed! The input was empty all along.
      // If we got here, at least one value was successfully extracted from
      // `attr_content`
      input_string_still_has_values = false;
      break;
    }
    std::cout << "   ----> Value extracted is " << single_value << '\n';
    // Store the single value into a vector & look for more.
    multiple_values.push_back(single_value);
  }

  // [2]: If we found only one value in the vector, we get rid of the vector and
  // store this single value in the ParamSet. Otherwise, we store the entire
  // vector.
  if (multiple_values.size() == 1) {
    single_value = multiple_values[0];
    ps->assign(attr_name, single_value);
  } else
    ps->assign(attr_name, multiple_values);

  return true;
}

/*!
 * This function will extract all the instances of **composite elements**
 * present in the input string `attr_content`. A composite element is a
 * homogeneous n-tuple of 2, 3 or 4 values of the same type, such as Point3f,
 * Vector2i, or Normal3f, for instance.
 *
 * @note
 * The composite type T **must have** the operator[]() implemented for this
 * function to work.
 *
 * @note
 * If the function finds only one instance of the composite element, then we
 * store a single value directly in the ParamSet, rather then a vector with just
 * one instance.
 *
 * @tparam T The basic type of the composit element.
 * @tparam N How many individual values each composite element has.
 * @param attr_name The attribute name.
 * @param attr_content The string attribute value we wish to convert.
 * @param[out] ps The output gc::ParamSet object.
 * @return A vector with all the composite elements extracted or no-value if
 * none is available.
 *
 * Example: A `Vector3f` has 3 elements of type `float`.
 * bl="255 255 51" will be stored as ps->m_map["bl"]=Color24(255,255,51) in the
 * `ParamSet` `ps`.
 */
template <typename T, std::uint16_t N>
bool convert(const std::string &attr_name, const std::string &attr_content,
             ParamSet *ps) {
  // assert(ps);
  std::istringstream iss{attr_content};
  std::vector<T> multiple_composite_values;
  T single_composite_value{};
  bool input_string_still_has_values{true};
  while (
      input_string_still_has_values &&
      !iss.eof()) // [1] Keep reading groups of N values from the input string.
  {
    for (std::uint16_t idx{0}; idx < N;
         ++idx) // Try to extract a N-tuple from the string.
    {
      iss >> single_composite_value[idx]; // Try to extract a value.
      if (iss.fail()) // Failed while extracting value this time around?
      {
        if (multiple_composite_values.empty()) // Completely empty?
          return false; // then, there is nothing to return.

        input_string_still_has_values = false;
        break; // There is something in the vector.
      }
    }
    // Add the newly extracted composite item to the result vector.
    multiple_composite_values.push_back(single_composite_value);
  }

  // [2] If we found only one value in the vector, we get rid of the vector and
  // store this single value in the ParamSet. Otherwise, we store the entire
  // vector.
  if (multiple_composite_values.size() == 1) {
    single_composite_value = multiple_composite_values[0];
    ps->assign(attr_name, single_composite_value);
  } else
    ps->assign(attr_name, multiple_composite_values);

  return true;
}

/*!
 * This function will extract all the instances of **composite elements**
 * present in the input string `attr_content`. A composite element is a
 * Color represented by Default[0, 255], Spectre[0, 1] and Hexadecimal values
 *
 *
 * @param attr_name The attribute name.
 * @param attr_content The string attribute value we wish to convert.
 * @param[out] ps The output gc::ParamSet object.
 * @return A vector with all the composite elements extracted or no-value if
 * none is available.
 *
 * Example: A `Color` has 3 elements of type `float`.
 * bl="255 255 51" will be stored as ps->m_map["bl"]=Color24(255,255,51) in the
 * `ParamSet` `ps`.
 */
template <>
bool convert<Color>(const std::string &attr_name, const std::string &attr_content, ParamSet *ps) {
    std::istringstream iss{attr_content};
    std::vector<Color> colors;
    std::string token;
    std::vector<std::string> tokens;

    while (iss >> token) 
        tokens.push_back(token);
    

    if (tokens.empty()) 
      return false;

    std::size_t i{0};
    while (i < tokens.size()) 
    {
        Color c;

        if (tokens[i][0] == '#') 
        {
            std::string hex = tokens[i].substr(1);
            if (hex.length() == 6) 
            {
                float r, g, b;
                std::sscanf(hex.c_str(), "%2f%2f%2f", &r, &g, &b);
                c = Color(r / 255.0f, g / 255.0f, b / 255.0f);
                colors.push_back(c);
                i++;
                continue;
            }
            return false; 
        }

        try 
        {
            float v1 = std::stof(tokens[i]);
            float v2 = v1, v3 = v1;

            std::size_t consumed{1};

            if (i + 2 < tokens.size() && tokens[i+1][0] != '#' && tokens[i+2][0] != '#') 
            {
                v2 = std::stof(tokens[i+1]);
                v3 = std::stof(tokens[i+2]);
                consumed = 3;
            }

            if (v1 > 1.0f || v2 > 1.0f || v3 > 1.0f) 
            {
                v1 /= 255.0f;
                v2 /= 255.0f;
                v3 /= 255.0f;
            }

            c = Color(v1, v2, v3);
            colors.push_back(c);
            i += consumed;

        } 
        catch (...) 
        {
            return false; 
        }
    }

    if (colors.empty()) 
      return false;

    if (colors.size() == 1) 
        ps->assign(attr_name, colors[0]);
    else 
        ps->assign(attr_name, colors);
    
    return true;
}

/// This is the list of all supported tags and their corresponding
/// attributes/type.
std::unordered_map<std::string, std::vector<std::string>> tag_catalog
{
    {
        "background",
        {
            "type",
            "filename",
            "color",
            "tl",
            "tr",
            "bl",
            "br",
        },
    },
    {
        "film",
        {
            "type",
            "w_res",
            "h_res",
            "filename",
            "img_type",
            "resolution",
            "diagonal",
            "scale",
            "crop_window",
            "gamma_corrected",
        },
    },
    {
        "filter",
        {
          "type",
          "alpha",
          "x_width",
          "y_width",
        }
    },
    {"camera",
     {
         "type",
         "shutter_open",
         "shutter_close",
         "lens_radius",
         "screen_window",
         "focal_distance",
         "fovy",
         "mapping",
     }},
    {"sampler",
     {
         "type",
         "samples_per_pixel",
         "n_sampled_dimensions",
         "x_samples",
         "y_samples",
         "jitter",
     }},
    {"lookat",  
     {
         "look_from",
         "look_at",
         "vup",
     }},
    {"material",
     {
         "type",
         "color_type",
         "color",
         "ka",
         "kd",
         "ks",
         "glossiness",
         //  "color_map",
         "mirror",
         "roughness",   
         "ior",         
        "eta",         
        "k",    
        "kd_texture",   
        "roughness_texture",   
        "ior_texture",         
        "eta_texture",         
        "k_texture",      
        "mat_type",
        "normal_map",
     }},
     {"emitter",
      {
        "type",
        "s",
        "i",
        "two_sided"
      }
     },
     {"make_named_emitter",
      {
        "name",
        "type",
        "i",
        "s",
        "two_sided"
      }
    },
    {"named_emitter",
      {
        "name",
      }
    },
    {"named_texture",
      {
        "name",
      }
    },
    {"make_named_texture",
      {
        "name",
        "type",
        "mapping",
        "filename",
        "data_type",
        "br",
        "bl",
        "tr",
        "tl",
        "value",
        "mode",
        "su",
        "sv",
        "trilinear",
        "max_anisotropy",
      }
    },
    {"object",
     {
         "type",
         /// Sphere
         "radius",
         "center",
         "zmin",
         "zmax",
         "phimax",
         /// Cylinder
         "height",
         /// Plane
         "point",
         "normal",
         /// TriangleMesh
          "indices",
          "ntriangles",
          "vertices",
          "vertex_indices",
          "normals",
          "normal_indices",
          "uvs",
          "uv_indices",
          "reverse_vertex_order",
          "compute_normals",
          "backface_cull",
          "swap_handedness",
          "filename",
     }},
    {"aggregator",
     {
         "type",
         "max_prims_per_node",
     }},
    {"integrator",
     {
         "type",
         "depth",
        //  "n_sampled_dimensions"
         //  "mapping_interval",
         //  "n_intervals",
     }},
    {"make_named_material",
     {
         "type",
         "name",
         "color",
         "ka",
         "kd",
         "ks",
         "glossiness",
         //  "color_map",
         "mirror",
         "roughness",   
         "ior",         
        "eta",         
        "k",           
        "kd_texture",   
        "roughness_texture",   
        "ior_texture",         
        "eta_texture",         
        "k_texture",      
        "mat_type",
        "normal_map",
     }},
    {"named_material",
     {
         "name",
     }},
    {"light_source",
     {
         "type",
         "i",
         "s",
         "from",
         "to",
         "direction",
         "attenuation",
          "cutoff",
          "falloff",
         "world_radius",
     }},
    {"include",
     {
         "filename",
     }},
    {
        "render_again",
        {""}, // no attributes
    },
    {
        "world_begin",
        {""}, // no attributes
    },
    {
        "world_end",
        {""}, // no attributes
    },
    {
        "push_ctm",
        {""},
    },
    {
        "pop_ctm",
        {""},
    },
    {
        "push_gs",
        {""},
    },
    {
        "pop_gs",
        {""},
    },

    {"translate",
     {
         "delta",
     }},
    {"identity",
     {
         "",
     }},
    {"rotate",
     {
         "angle",
         "axis",
     }},
    {"scale",
     {
         "delta",
     }},
    {"object_instance_begin",
     {
         "name",
     }},
    {"object_instance_end",
     {
         "",
     }},
    {"object_instance_call",
     {
         "name",
     }},
};

std::unordered_map<std::string, std::vector<std::string>> similar_tags
{

};

/// Maps the tag name to its corresponding API function.
std::unordered_map<std::string, std::function<void(const ParamSet &)>>
    api_functions{

        {"background", API::background}, {"camera", API::camera},
        {"lookat", API::lookAt},        {"world_begin", API::worldBegin},
        {"world_end", API::worldEnd},   {"film", API::film},
        {"filter", API::filter},
        {"material", API::material},     {"object", API::object},
        {"integrator", API::integrator}, {"sampler", API::sampler},
        {"make_named_material", API::makeNamedMaterial},
        {"emitter", API::emitter}, {"make_named_emitter", API::makeNamedEmitter},
        {"named_emitter", API::namedEmitter},
        {"named_material", API::namedMaterial}, {"light_source",
        API::lightSource},
        {"named_texture", API::namedTexture}, {"make_named_texture", API::makeNamedTexture},
        {"aggregator", API::aggregator}, {"translate", API::translate},
        {"rotate", API::rotate}, {"scale", API::scale},
        {"identity", API::identity}, 
        {"object_instance_begin", API::objInstanceBegin},
        {"object_instance_end", API::objInstanceEnd},
        {"object_instance_call", API::objInstanceCall},
        {"push_ctm", API::pushCTM}, {"pop_ctm", API::popCTM},
        {"push_gs", API::pushGS}, {"pop_gs", API::popGS}
    };

/// Maps convertion function to an attribute name.
std::unordered_map<std::string, ConverterFunction> converters{
    {"type", convert<std::string>}, // "type" must be a string.
    {"name", convert<std::string>}, // "name" must be a string.
    
    //
    {"color", convert<Color>}, // "color" is a Color24 with 3 fields.
    // Background attributes.
    {"bl", convert<Color>},
    {"tl", convert<Color>},
    {"tr", convert<Color>},
    {"br", convert<Color>},
    // Camera attributes
    {"lensradius", convert<float>},
    {"shutteropen", convert<float>},
    {"shutterclose", convert<float>},
    {"fovy", convert<float>},
    {"mapping", convert<std::string>},
    {"screen_window", convert<Point4, 4>},
    // Camera attributes but at lookat tag
    {"look_from", convert<Point3>},
    {"look_at", convert<Point3>},
    {"vup", convert<Vec3>},
    {"shutter_open", convert<float>},
    {"shutter_close", convert<float>},
    {"lens_radius", convert<float>},
    {"focal_distance", convert<float>},
    // Filter attributes
    {"x_width", convert<float>},
    {"y_width", convert<float>},
    {"alpha", convert<float>},
    // Film attributes
    {"w_res", convert<int>},
    {"h_res", convert<int>},
    {"resolution", convert<Point2, 2>},
    {"diagonal", convert<float>},
    {"scale", convert<float>},
    {"crop_window", convert<Point4, 4>},
    {"filename", convert<std::string>},
    {"img_type", convert<std::string>},
    {"gamma_corrected", convert<std::string>},
    // Material attributes
    {"mirror", convert<Color>},
    {"ka", convert<Color>},
    {"kd", convert<Color>},
    {"ks", convert<Color>},
    {"glossiness", convert<float>},
    {"roughness", convert<float>},
    {"ior", convert<float>},
    {"eta", convert<Color>},      
    {"k", convert<Color>},

    {"roughness_texture", convert<std::string>},
    {"ior_texture", convert<std::string>},
    {"eta_texture", convert<std::string>},
    {"k_texture", convert<std::string>},
    {"kd_texture", convert<std::string>},
    {"trilinear", convert<std::string>},
    {"max_anisotropy", convert<float>},

    {"mat_type", convert<std::string>},
    {"data_type", convert<std::string>},
    {"mapping", convert<std::string>},
    {"normal_map", convert<std::string>},
    {"mode", convert<std::string>},
    {"value", convert<Color>},
    {"su", convert<float>},
    {"sv", convert<float>},
    // Object attributes
    {"radius", convert<float>},
    {"center", convert<Point3>},
    {"phimax", convert<float>},
    {"height",convert<float>},

    {"point", convert<Point3>},
    {"normal", convert<Normal3>},

    {"indices", convert<int>},
    {"ntriangles", convert<int>},
    {"vertices", convert<Point3>},
    {"vertex_indices", convert<int>},
    {"normals", convert<Normal3>},
    {"normal_indices", convert<int>},
    {"uvs", convert<Point2>},
    {"uv_indices", convert<int>},
    {"reverse_vertex_order", convert<std::string>},
    {"compute_normals", convert<std::string>},
    {"backface_cull", convert<std::string>},
    {"swap_handedness", convert<std::string>},
    // Integrator
    {"zmin", convert<float>},
    {"zmax", convert<float>},
    {"near_color", convert<Color>},
    {"far_color", convert<Color>},
    {"n_sampled_dimensions", convert<int>},
    {"mapping_interval", convert<float>},
    {"n_intervals", convert<int>},
    {"depth", convert<int>},

    // Light attributes
    {"i", convert<Color>},
    {"s", convert<Color>},
    {"from", convert<Point3>},
    {"to", convert<Point3>},
    {"position", convert<Point3>},
    {"direction", convert<Vec3>},
    {"attenuation", convert<Vec3>},
    {"cutoff", convert<float>},
    {"falloff", convert<float>},
    {"world_radius", convert<float>},
    {"axis", convert<Point3>},
    {"angle", convert<float>},
    {"two_sided", convert<std::string>},

    // Sampler attributes
    {"samples_per_pixel", convert<int>},
    {"x_samples", convert<int>},
    {"y_samples", convert<int>},
    {"jitter", convert<std::string>},

    // Transform attributes
    {"delta", convert<Point3>},

    // Aggregator attributes
    {"max_prims_per_node", convert<int>},
};

/*!
 * This function checks if the tag received is valid.
 * @param tag_name The tag name we want to validate.
 */
bool is_valid_tag(std::string_view tag_name) {
  // Check if we have a valid registered tag name.
  auto tag_query{tag_catalog.find((std::string)tag_name)};
  return tag_query != tag_catalog.end();
}

/*!
 * This function checks if the attribute name belongs to a given tag name.
 * @note The precondition is that tag_name is valid.
 * @param tag_name A valid tag name.
 * @attribute_name The attribute name we want to validate.
 */
bool is_valid_attribute(std::string_view tag_name,
                        std::string_view attribute_name) {
  // Get the attribute list associated with `tag_name`.
  auto attribute_list{tag_catalog[(std::string)tag_name]};
  auto attr_query =
      std::find(attribute_list.begin(), attribute_list.end(), attribute_name);
  return attr_query != attribute_list.end();
}

/*!
 * This function invokes a converter function that translates the attribute
 * content (as a string) into the expected type and store it into the
 * gc::ParamSet object received as input argument.
 * @param attr_name The attribute name.
 * @param attr_content The attribute value as a string.
 * @param ps A reference to the current gc::ParamSet object we are filling in.
 */
void Parser::parse_attribute(const std::string &attr_name /* IN value */,
                     const std::string &attr_content /* IN value */,
                     ParamSet *ps /* OUT value*/) {
  std::ostringstream oss;
  // Find the proper convertion function.
  auto converter_func = converters[attr_name];
  if (converter_func) {
    if (converter_func(attr_name, attr_content, ps)) {
      oss << " ⁺ Successfuly converted attribute " << std::quoted(attr_name);
      MESSAGE(oss.str());
    } else {
      oss << " - Convertion of " << std::quoted(attr_name) << " failed!";
      MESSAGE(oss.str());
    }
  } else {
    oss << " - Could not find a convertion function for the tag "
        << std::quoted(attr_name) << ". Skipping...";
    WARNING(oss.str());
  }
}

/*!
 * This is the entry point where the parsing of the scene file begins.
 */
  void Parser::parse_scene(const std::string &filename) 
  {
      tinyxml2::XMLDocument doc;
      auto err = doc.LoadFile(filename.c_str());
      if (err != tinyxml2::XML_SUCCESS) 
      {
        std::cerr << "Error loading the XML file '" << filename << "': "
                  << doc.ErrorIDToName(err);
        if (doc.ErrorStr())
            std::cerr << " (" << doc.ErrorStr() << ")";
        std::cerr << '\n';
        return;
      }
  

  // [2] Get the Root node
  tinyxml2::XMLElement *root = doc.RootElement();
  if (root == nullptr) {
    std::cerr << "Root node of the XML tree was not found!" << '\n';
    return;
  }

  // [3] Iterate over every child elements, i.e. over every tag.
  for (tinyxml2::XMLElement *child_node = root->FirstChildElement();
       child_node != nullptr; child_node = child_node->NextSiblingElement()) {
    // ================================================================================
    // Validate the current tag name.
    // --------------------------------------------------------------------------------
    std::string tag_name = str_to_lower(child_node->Name());
    if (not is_valid_tag(tag_name)) {
      std::ostringstream oss;
      oss << "The tag " << std::quoted(tag_name) << " is not valid!";
      WARNING(oss.str());
      continue; // Skip to the next tag in the scene file.
    }

    {
      std::ostringstream oss;
      oss << ">>>>> Started parsing tag " << std::quoted(tag_name) << ".";
      MESSAGE(oss.str());
    }
    // ================================================================================
    // At this point we have a valid tag name. Now we need to validate its
    // attributes.
    // --------------------------------------------------------------------------------
    // Create the empty gc::ParamSet object to store the attributes we will
    // process next.
    ParamSet ps;
    // Iterate over this tag's attributes
    for (const tinyxml2::XMLAttribute *attr = child_node->FirstAttribute();
         attr != nullptr; attr = attr->Next()) {
      // Validate the current attribute name.
      std::string attribute_name{str_to_lower(attr->Name())};
      if (not is_valid_attribute(tag_name, attribute_name)) {
        std::ostringstream oss;
        oss << "The tag " << std::quoted(tag_name)
            << " does not have an attribute " << std::quoted(attribute_name)
            << ". Ignoring...";
        WARNING(oss.str());
        continue; // Skip to the next attribute inside this tag.
      }
      // Parse the string version of this attribute into its expected value.
      // The result is stored inside the gc::ParamSet object, passed in as the
      // last argument.
      std::string attribute_value{str_to_lower(attr->Value())};
      parse_attribute(attribute_name, attribute_value, /*OUT value*/ &ps);
    }
    // ================================================================================
    // Now we have gc::ParamSet object filled in and ready to be passed along to
    // the API.
    // ================================================================================
    // ============================================================================
    /// HACK: If the tag is `include` we call `parse_scene_file()` recursively.
    // ----------------------------------------------------------------------------
    if (tag_name == "include") {
      auto filename = ps.retrieve<std::string>("filename", "");
      if (filename.empty()) {
        WARNING("Missing attribute \"filename\" in tag \"include\"");
        continue;
      }
      if (not fs::exists(fs::path{filename.c_str()})) {
        std::ostringstream oss;
        oss << "Included file " << std::quoted(filename) << " does not exist.";
        ERROR(oss.str());
      }
      // Recursive call to process subfile.

      parse_scene(filename.c_str());
      continue; // This tag doesn't have an API function associated with; get
                // next tag.
    }
    if (tag_name == "render_again") {
      std::ostringstream oss;
      oss << "<<<<< <render_again> tag was founded! Starting the Rendering "
             "phase."
          << ".\n";
      MESSAGE(oss.str());
      API::apiState = ApiState::WORLD_BLOCK;
      API::worldEnd(ps);
      continue;
    }
    // ============================================================================

    // Check whether this tag_name has a proper API function.
    if (api_functions.count(tag_name) == 0) {
      std::ostringstream oss;
      oss << "The tag " << std::quoted(tag_name)
          << " does not have a corresponding API function associated with. "
             "Ignoring...";
      WARNING(oss.str());
      continue;
    }

    {
      std::ostringstream oss;
      oss << "<<<<< Calling API function for the tag " << std::quoted(tag_name)
          << ".\n";
      MESSAGE(oss.str());
    }
    // Call the api function associated with the tag name.
    api_functions[tag_name](ps);
  }
}
}; // namespace ssrt