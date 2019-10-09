# What is this?

This is my file compressor. It compresses files. It is hot garbage, but I also think it is the best thing I have created so far because it can have a practical use. please don't use it practically though.

# How to use

 *  ``` -c ```: 
    Specify the file to compress. First argument starts in this mode, so it does not need restating.

 *  ``` -d ```:
    Specify the file to decompress.

 *  ``` -o ```:
    Specify the file to output to. Default is test.cmp for compressing and test.dcmp for decompressing.

 *  ``` -C ```:
    Specify chunk size in bits. Default is 8. (See Funky features -> Chunk size for more detail)

## Examples
``` cmprssr ./MyFile.txt -o ./myCompressed.cmp ``` compresses MyFile.txt and puts the data in myCompressed.cmp.

``` cmprssr -C 16 -c ./MyOtherFile.someExtention ``` compresses MyOtherFile.someExtention with a chunk size of 16 (which may compress slightly better)

``` cmprssr -d ./test.cmp ``` decompresses test.cmp (it infers chunk size from the file)

# Funky features

### Chunk size

You know huffman trees? Normally they store ASCII. I got a little obsessive with templating and it can take any size of data (not just 8 bits). When compressing, you can specify the chunk size with -C and the number of bits (right now the supported numbers are 8, 16, 32, 64).

# Some data

I used it on the bee movie script and this was the result:

| File         | Size      |
| ------------ | --------- |
| Original     | 50140 b   |
| Compressed   | 26976 b   |
| Decompressed | 50142 b   |

I think I know where those 2 extra bytes came from. This is still a work in progress (but isn't all code?).