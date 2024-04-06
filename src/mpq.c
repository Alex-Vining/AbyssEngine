#include "mpq.h"
#include "log.h"
#include "crypto.h"
#include <stdlib.h>
#include <string.h>

mpq_t* mpq_load(const char* mpq_path) {
    LOG_DEBUG("Loading '%s'...", mpq_path);

    mpq_t* result = malloc(sizeof(mpq_t));
    memset(result, 0, sizeof(mpq_t));
    
    result->file = fopen(mpq_path, "rb");
    if (result->file == NULL) {
        LOG_FATAL("Failed to load '%s'!", mpq_path);
    }

    mpq_header_read(result->file, mpq_path, &result->header);   
    result->hashes = mpq_hash_read_table(result->file, mpq_path, &result->header);
    result->blocks = mpq_block_read_table(result->file, mpq_path, &result->header);

    return result;
}

void mpq_free(mpq_t* mpq) {
    fclose(mpq->file);
    free(mpq->blocks);
    free(mpq->hashes);
    free(mpq);
}

bool mpq_file_exists(mpq_t* mpq, const char* file_path) {
    return mpq_get_file_hash(mpq, file_path) != NULL;
}

mpq_hash_t* mpq_get_file_hash(mpq_t* mpq, const char* file_path) {
    uint64_t file_hash = crypto_hash_file_name(file_path);
    
    for (int i=0; i<mpq->header.hash_table_entries; i++) {
        if (mpq->hashes[i].key != file_hash) {
            continue;
        }
        
        return &mpq->hashes[i].hash;
    }
    return NULL;
}

void* mpq_load_entire_file(mpq_t* mpq, const char* file_path) {
    return NULL;
}
