import unicodedata

def remove_diacritics(text):
    # Normalize to NFD to split base letters and accents
    normalized = unicodedata.normalize('NFD', text)
    # Keep only base characters (i.e., remove accents)
    return ''.join(c for c in normalized if unicodedata.category(c) != 'Mn')

def convert_unicode_to_ascii_file(input_path, output_path):
    # Read the file (assumes UTF-8 encoding)
    with open(input_path, 'r', encoding='utf-8') as infile:
        text = infile.read()

    # Remove accents and non-ASCII characters
    ascii_text = remove_diacritics(text)

    # Optional: replace special Spanish characters manually
    ascii_text = ascii_text.replace('ñ', 'n').replace('Ñ', 'N')

    # Remove any remaining non-ASCII characters (safely)
    ascii_text = ascii_text.encode('ascii', 'ignore').decode('ascii')

    # Write to output file using ASCII encoding
    with open(output_path, 'w', encoding='ascii') as outfile:
        outfile.write(ascii_text)

    print("File successfully converted to ASCII.")

# Example usage
convert_unicode_to_ascii_file("el_quijote.txt", "quijote_ascii.txt")
