# LSB Steganography - Source Code & Implementation

This directory contains the complete C implementation of the **Least Significant Bit (LSB) Image Steganography** algorithm for 24-bit uncompressed BMP images.

---

## 📁 Source File Breakdown

| File | Type | Description |
| :--- | :--- | :--- |
| **`test_encode.c`** | Source | Application entry point (`main`), command-line argument validation (`-e` / `-d`), and high-level workflow orchestration. |
| **`encode.c`** | Source | Implementation of encoding operations: BMP header copying, magic string embedding (`#*`), extension handling, secret file size calculation, and LSB data encoding. |
| **`encode.h`** | Header | Prototypes, constants (`MAX_SECRET_BUF_SIZE`, `MAX_IMAGE_BUF_SIZE`), and the `EncodeInfo` structure definition. |
| **`decode.c`** | Source | Implementation of decoding operations: BMP header skipping, magic string verification, metadata recovery, and file extraction. |
| **`decode.h`** | Header | Prototypes and the `DecodeInfo` structure definition for extraction. |
| **`common.h`** | Header | Shared definitions, including `MAGIC_STRING` (`#*`). |
| **`types.h`** | Header | User-defined typedefs and status enums (`Status`, `OperationType`, `uint`). |

### Included Test Files
- **`beautiful.bmp`**: Sample 24-bit uncompressed source BMP image used as the carrier.
- **`secret.txt`**: Sample input text file containing the confidential payload to conceal.
- **`stego.bmp`**: Sample generated stego image containing encoded data.

---

## 🔨 Compilation

Compile all source files directly in this directory using `gcc`:

```bash
# Recommended quick compilation:
gcc *.c

# Or compile with custom binary name:
gcc -Wall -Wextra *.c -o stego
# (or explicitly: gcc -Wall -Wextra test_encode.c encode.c decode.c -o stego)
```

---

## 🚀 Running the Program

### 1. Encode Secret Data (`-e`)

Embeds `secret.txt` into `beautiful.bmp` and produces an output stego image (`stego.bmp`):

```bash
# General syntax:
./a.out -e <source_image.bmp> <secret_file.txt> [output_stego_image.bmp]
# (or ./stego if compiled with -o stego)

# Example using sample files:
./a.out -e beautiful.bmp secret.txt stego.bmp
```

> **Note**: If the output image name is omitted, it defaults to `default.bmp`.

---

### 2. Decode Secret Data (`-d`)

Extracts the concealed secret file from `stego.bmp`:

```bash
# General syntax:
./a.out -d <stego_image.bmp> [output_file]
# (or ./stego if compiled with -o stego)

# Example:
./a.out -d stego.bmp output.txt
```

> **Note**: If the output file name is omitted, the extracted file is saved as `output` followed by the original encoded extension (e.g., `output.txt`).

---

## 🧪 Testing & Verification

Verify that the extracted file is identical to the original secret file:

**Linux / macOS:**
```bash
diff secret.txt output.txt
```

**Windows (PowerShell):**
```powershell
Compare-Object (Get-Content secret.txt) (Get-Content output.txt)
```

If no output is returned, the encoding and decoding succeeded with 100% data integrity.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

