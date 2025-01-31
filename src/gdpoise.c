#include <elf.h>
#include <linux/limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int read_elf(char *filename) {
  FILE *fptr = fopen(filename, "rb");
  if (!fptr) {
    printf("Error opening file %s\n", filename);
    return -1;
  }

  // Load ELF header struct
  Elf64_Ehdr hdr;
  size_t header_size = sizeof(Elf64_Ehdr);

  if (fread(&hdr, header_size, 1, fptr) != 1) {
    printf("Error reading ELF Header of file %s\n", filename);
    fclose(fptr);
    return -1;
  }

  // Check if File is type ELF by examining magic values
  if (hdr.e_ident[EI_MAG0] != ELFMAG0 || hdr.e_ident[EI_MAG1] != ELFMAG1 ||
      hdr.e_ident[EI_MAG2] != ELFMAG2 || hdr.e_ident[EI_MAG3] != ELFMAG3) {
    printf("Error, %s is not an ELF file. Closing..\n", filename);
    fclose(fptr);
    return -1;
  }

  printf("\nELF Header Information:\n");
  printf("------------------------\n");
  printf("Number of sections in header:      %d\n", hdr.e_shnum);
  printf("Start of section header:      %ld\n", hdr.e_shoff);
  printf("Location of Strings defined in header table: %ld\n", hdr.e_shstrndx);

  fclose(fptr);
  return 0;
}

int write_elf(char *input_file, char *output_file, uint16_t new_shnum,
              uint16_t new_shoff, uint16_t new_shstrndx) {
  FILE *infile = fopen(input_file, "rb");
  if (!infile) {
    printf("Error while reading file %s\n", input_file);
    return -1;
  }

  // Get the size of the input file
  fseek(infile, 0, SEEK_END);
  long file_size = ftell(infile);
  fseek(infile, 0, SEEK_SET);

  // Allocate memory to read the entire ELF file
  unsigned char *file_data = (unsigned char *)malloc(file_size);
  if (!file_data) {
    printf("Error allocating memory for file data\n");
    fclose(infile);
    return -1;
  }

  // Read the entire file into memory
  if (fread(file_data, 1, file_size, infile) != file_size) {
    printf("Error reading the entire file %s\n", input_file);
    free(file_data);
    fclose(infile);
    return -1;
  }

  fclose(infile);

  // Load ELF header struct from the file data
  Elf64_Ehdr *hdr = (Elf64_Ehdr *)file_data;

  // Check if the file is ELF
  if (hdr->e_ident[EI_MAG0] != ELFMAG0 || hdr->e_ident[EI_MAG1] != ELFMAG1 ||
      hdr->e_ident[EI_MAG2] != ELFMAG2 || hdr->e_ident[EI_MAG3] != ELFMAG3) {
    printf("Error, %s is not an ELF file. Closing..\n", input_file);
    free(file_data);
    return -1;
  }

  // Modify the ELF header
  hdr->e_shnum = new_shnum;
  hdr->e_shoff = new_shoff;
  hdr->e_shstrndx = new_shstrndx;

  // Open the output file for writing
  FILE *outfile = fopen(output_file, "wb");
  if (!outfile) {
    printf("Error while creating outfile %s\n", output_file);
    free(file_data);
    return -1;
  }

  // Write the entire modified file data (header + sections) to the output file
  if (fwrite(file_data, 1, file_size, outfile) != file_size) {
    printf("Error writing the modified data to output file %s\n", output_file);
    free(file_data);
    fclose(outfile);
    return -1;
  }

  fclose(outfile);
  printf("Setting execution permissions...\n");
  chmod(output_file,
        S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  free(file_data);
  return 0;
}

void usage(char *command) {
  printf("----------------------------------------\n");
  printf("Usage: %s <EXECUTABLE>\n", command);
  printf("----------------------------------------\n");

  exit(1);
}

int main(int argc, char *argv[]) {
  char *input_file = NULL;
  char *output_path = NULL;

  for (int i; i < argv; i++) {
    if (strcmp(argv[i], "-h") == 0) {
      usage(argv[0]);
    } else if (strcmp(argv[i], "-o") == 0) {
      if (i + 1 < argc) {
        output_path = argv[i + 1];
        i++;
      } else {
        printf("Error: -o requires an output path\n");
        usage(argv[0]);
      }
    } else if (input_file == NULL) {
      input_file = argv[1];
    } else {
      printf("Error: Unexpected argument '%s'\n", argv[1]);
      usage(argv[0]);
    }
  }
  if (input_file == NULL) {
    printf("Error: No input file specified\n");
    usage(argv[0]);
  }

  if (argc < 2) {
    usage(argv[0]);
  }
  char *name = argv[1];
  if (strcmp(name, "-h") == 0) {
    usage(argv[0]);
  }
  char *path = argv[3];
  if (argc > 2) {
    printf("Error, too many arguments provided\n");
  }

  if (read_elf(input_file) == 0) {
    return -1;
  }

  char output_file[PATH_MAX];
  if (output_path != NULL) {
    snprintf(output_file, sizeof(output_file), "%s", output_path);
  } else {
    snprintf(output_file, sizeof(output_file), "%s_modified", input_file);
  }
  uint16_t shnum = 0xffff;

  uint16_t shoff = 0xffff;

  uint16_t shstrndx = 0xffff;

  write_elf(argv[1], output_file, shnum, shoff, shstrndx);
}
