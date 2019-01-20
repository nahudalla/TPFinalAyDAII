const FS = require("fs");

const OUTPUT_DIR = "web-build-release";
const OUTPUT_FILE = `${OUTPUT_DIR}/package.json`;

if (!FS.existsSync(OUTPUT_DIR)){
  FS.mkdirSync(OUTPUT_DIR);
}

// Read original project package.json
const original_config = JSON.parse(FS.readFileSync("./package.json"));

// Extract configuration from project's config
const config = original_config.electronPackageJSONGenerator || {};

// Extract which keys to exclude from the project config
const exclude = config.exclude || [];
exclude.push("electronPackageJSONGenerator");

// Extract which keys and values to add to the generated config
const append = config.append || {};

// This variable holds the generated config
const generated_config = {};

// Generate new configuration from the original, excluding the specified keys
Object.keys(original_config).forEach(key => {
  if(exclude.includes(key)) return;

  generated_config[key] = original_config[key];
});

// Append additional keys to generated config
Object.keys(append).forEach(key => {
  generated_config[key] = append[key];
});

// Write generated configuration
FS.writeFileSync(OUTPUT_FILE, JSON.stringify(generated_config, null, 2));
