#include <zlib.h>
#include <stdio.h>
#include "kseq.h"
#include "uthash.h"

#include "uthash.h"

struct my_struct {
    char key[255];                    /* key */
    //int count;
    FILE *fptr;
    UT_hash_handle hh;                /* makes this structure hashable */
};

struct my_struct *barcodes = NULL;    /* important! initialize to NULL */

void add_barcode(char *key) {
//struct my_struct * add_barcode(char *key) {
    struct my_struct *s;

    s = malloc(sizeof(struct my_struct));
    strcpy(s->key, key);
    s->fptr = fopen(strcat(key, ".fastq"), "w+");;

    HASH_ADD_STR( barcodes, key, s );  /* id: name of key field */
}

void delete_barcode(struct my_struct *key) {
    HASH_DEL(barcodes, key);           /* user: pointer to deletee */
    free(key);                         /* optional; it's up to you! */
}


KSEQ_INIT(gzFile, gzread)

void remove_newline(char *line){
    int new_line = strlen(line) -1;
    if (line[new_line] == '\n')
        line[new_line] = '\0';
}

int main(int argc, char *argv[])
{
	gzFile fp;
	kseq_t *seq;
	struct my_struct *s;
	int i, l;
	char delim[] = ":";
	int pos = 4;


	if (argc < 3) {
		fprintf(stderr, "Usage: %s <barcodes.txt> <in.seq>\n", argv[0]);
		return 1;
	}


	// read in barcodes
	char const* const barcode_file_name = argv[1];
	FILE* barcode_file_pointer = fopen(barcode_file_name, "r");
	char line[256];
	while (fgets(line, sizeof(line), barcode_file_pointer)) {
		remove_newline(line);
		HASH_FIND_STR( barcodes, line, s);
		if(s == NULL){
			add_barcode(line);
		}
	}

	// read in fastq
	int line_format;
	line_format = 0;
	fp = gzopen(argv[2], "r");
	seq = kseq_init(fp);
	while ((l = kseq_read(seq)) >= 0) {

		// parse the comment
		i = 1;	
		char comment[255];
		strcpy(comment, seq->comment.s);
		char *ptr = strtok(comment, delim);
		while(ptr != NULL && i < pos){
			ptr = strtok(NULL, delim);
			i++;
		}
		
		HASH_FIND_STR( barcodes, ptr, s);
		if(s != NULL){
			fprintf(s->fptr, "%c%s", seq->qual.l == seq->seq.l? '@' : '>', seq->name.s);
			if (seq->comment.l) fprintf(s->fptr," %s", seq->comment.s);
			fputc('\n', s->fptr);
			/*
			for (i = 0; i < l; ++i) {
				fputc(seq->seq.s[i], s->fptr);
			}
			*/
			fprintf(s->fptr, "%s", seq->seq.s);
			fputc('\n', s->fptr);
			if (seq->qual.l != seq->seq.l) continue;
			fprintf(s->fptr, "+");
			fprintf(s->fptr, "%s", seq->qual.s);
			/*
			for (i = 0; i < l; ++i) {
				if (i == 0 || (line_format > 0 && i % line_format == 0)) fputc('\n', s->fptr);
				fputc(seq->qual.s[i], s->fptr);
			}
			*/
			fputc('\n', s->fptr);
		}
	}
	kseq_destroy(seq);
	gzclose(fp);

	return 0;
}
