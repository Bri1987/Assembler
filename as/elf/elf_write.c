//
// Created by 12167 on 2023/10/1.
//

#include "elf_write.h"

// write_ehdr() fills in the section header offset and writes
// the executable header to the file
static bool write_ehdr(elf_context *elf, FILE *f) {
    // Count up the header lengths. Must be final and aligned
    int shoff = sizeof(Elf32_Ehdr);
    for (int s = 0; s < NUM_SECTIONS; s++)
        shoff += elf->sections[s].logical_len;
    elf->ehdr.e_shoff = shoff;

    return 1 == fwrite(&elf->ehdr, sizeof(Elf32_Ehdr), 1, f);
}


// write_sdata() loops over the elf_sections and writes their
// data into the body of each section
static bool write_sdata(elf_context *elf, FILE *f) {
    for (int s = 0; s < NUM_SECTIONS; s++) {
        elf_section *sec = &elf->sections[s];

        // The null and .bss sections never have a length or body
        // In our little programs .data is also empty
        if (0 == sec->logical_len)
            continue;

        size_t n_written = fwrite(sec->data, sizeof(unsigned char), sec->logical_len, f);
        if (n_written != sec->logical_len) {
            printf("write_sdata: fwrite wrote %zu bytes but expected %d\n",
                   n_written, sec->logical_len);
            return false;
        }
    }
    return true;
}


// write_shdrs() writes the section header table to the end of the file
static bool write_shdrs(elf_context *elf, FILE *f) {
    size_t n_written = fwrite(&elf->shdrs, sizeof(Elf32_Shdr), NUM_SECTIONS, f);
    if (NUM_SECTIONS != n_written) {
        printf("write_shdrs: fwrite wrote %zu bytes but expected %lu\n",
               n_written, sizeof(Elf32_Shdr) * NUM_SECTIONS);
        return false;
    }
    return true;
}

// elf_write_file() is the public way to write the ELF structures to disk
bool elf_write_file(elf_context *elf, FILE *f) {
    if (!elf || !f)
        return false;


    // Some section data (alignment and offsets) can only be added after
    // we know we're done adding instructions and labels.
    finalize_sections(elf);  // Must do sections before section headers
    finalize_shdrs(elf);

    // Write the core ELF structures to the file
    if (!write_ehdr(elf, f))
        return false;
    if (!write_sdata(elf, f))
        return false;
    if (!write_shdrs(elf, f))
        return false;

    return true;
}