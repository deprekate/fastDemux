# fastDemux
A small utility to VERY quickly demux a fastq file

### Install

```sh
 git clone https://github.com/deprekate/fastDemux.git
 cd fastDemux; make
```

Optionally then copy the compiled binary to the bin folder or somewhere else that is on your PATH

### Running
To run the program, specify a barcodes file and a fastq file
```sh
./fastDemux <barcodes.txt> <in.seq>
```


--------------
## Example

The included fastq file has four reads from one barcoded sample and four reads from another, with the
barcodes of interest in the barcodes.txt file.

```sh
./fastDemux barcodes.txt test.fastq
```

### Output
Output is two files (AAGAGGCA+ACTCTAGG.fastq, TCCTGAGC+ACTCTAGG.fastq) that each contain four reads

