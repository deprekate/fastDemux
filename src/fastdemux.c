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

//void add_barcode(char *key) {
struct my_struct * add_barcode(char *key) {
    struct my_struct *s;

    s = malloc(sizeof(struct my_struct));
    strcpy(s->key, key);
    s->fptr = fopen(strcat(key, ".fastq"), "w+");;

    HASH_ADD_STR( barcodes, key, s );  /* id: name of key field */

    return s;
}

void delete_barcode(struct my_struct *key) {
    HASH_DEL(barcodes, key);           /* user: pointer to deletee */
    free(key);                         /* optional; it's up to you! */
}


KSEQ_INIT(gzFile, gzread)


int main(int argc, char *argv[])
{
	gzFile fp;
	kseq_t *seq;
	struct my_struct *s;
	int i, l, line;
	char delim[] = ":";
	int pos = 4;
	line = 0;


	if (argc < 3) {
		fprintf(stderr, "Usage: %s <barcodes.txt> <in.seq>\n", argv[0]);
		return 1;
	}

	fp = gzopen(argv[2], "r");
	seq = kseq_init(fp);
	while ((l = kseq_read(seq)) >= 0) {

		i = 1;	
		char *ptr = strtok(seq->comment.s, delim);
		while(ptr != NULL && i < pos){
			ptr = strtok(NULL, delim);
			i++;
		}
		HASH_FIND_STR( barcodes, ptr, s);
		if(s == NULL){
			s = add_barcode(ptr);
			//HASH_FIND_STR( barcodes, ptr, s);

			fprintf(s->fptr, "%c%s", seq->qual.l == seq->seq.l? '@' : '>', seq->name.s);

			/*
			if (seq->comment.l) fprintf(s->fptr," %s", seq->comment.s);
			fputc('\n', s->fptr);
			for (i = 0; i < l; ++i) {
				fputc(seq->seq.s[i], s->fptr);
			}
			fputc('\n', s->fptr);
			if (seq->qual.l != seq->seq.l) continue;
			fprintf(s->fptr, "+");
			for (i = 0; i < l; ++i) {
				if (i == 0 || (line > 0 && i % line == 0)) fputc('\n', s->fptr);
				fputc(seq->qual.s[i], s->fptr);
			}
			fputc('\n', s->fptr);
			*/
		}else{
			//add_kmer(kmer, s->count+1, line);
    			//strcpy(s->readname[s->count], line);
			//s->count++;
		}


		//if (seq->comment.l) printf("comment: %s\n", seq->comment.s);
	}
	kseq_destroy(seq);
	gzclose(fp);
	return 0;
}
