import os

# Input and output folders
input_folder = "data"
output_folder = "src/html_embedded"

# Make output folder if it doesn't exist
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# Go through each file in the input folder
for file in os.listdir(input_folder):
    if file.endswith(".html"):
        # Read the HTML file
        with open(os.path.join(input_folder, file), "r") as html_file:
            html_content = html_file.read()

        # Create a valid C variable name
        variable_name = file.replace(".", "_")

        # Create a header file with the HTML as a C string
        header_path = os.path.join(output_folder, variable_name + ".h")
        with open(header_path, "w") as header_file:
            header_file.write(f'const char {variable_name}[] PROGMEM = R"rawliteral(\n{html_content}\n)rawliteral";\n')
            print(f"Created header file: {header_path}")

