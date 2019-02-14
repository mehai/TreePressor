# TreePressor
Designed and implemented an application serving functions of compression, decompression, mirroring on square PPM images. Based on quadtree data structure. Technology used: C language.

### Installation

```
git clone https://github.com/mehai/TreePressor.git
make 
```
### How it works!

  General command: 
  ./quadtree [-c ratio | -d | -m type | -o] [input_file_1] [input_file_2](only for -o) [output_file]
  
*  -c grade = Compress the input_file_1 having image compression ratio [ratio];
*  -d = Decompress the input_file_1;
*  -m type = Mirror the input_file_1, [type] could be v(vertically) or h(horizontally). There could be an extra argument: [ratio] for image compression ratio;
*  -o = Overlay the input_file_1 and input_file_2 images. There could be an extra argument: [ratio] for image compression ratio.

### Examples

  For the next examples follow the files in examples directory.
  
>  Compression examples:
```
Command 1: ./quadtree -c 0 test0.ppm compress0_0.out
Output file: compress0_0.out

Command 2: ./quadtree -c 370 test1.ppm compress1_370.out
Output file: compress1_370.out
```
>  Decompression example:
```  
Command 3: ./quadtree -d compress1_370.out decompress1.ppm
Output file: decompress1.ppm
```
>  Mirror examples:
 ``` 
Command 4: ./quadtree -m h 0 test0.ppm mirror0_h_0.ppm
Output file: mirror0_h_0.ppm

Command 5: ./quadtree -m v 0 test0.ppm mirror0_v_0.ppm
Output file: mirror0_v_0.ppm

Command 6: ./quadtree -m h 370 test1.ppm mirror1_h_370.ppm
Output file: mirror1_h_370.ppm

Command 7: ./quadtree -m v 370 test1.ppm mirror1_v_370.ppm
Output file: mirror1_v_370.ppm
```
>  Overlay example:
 ``` 
Command 8: ./quadtree -o 100 test0.ppm test1.ppm overlay_100_test0_test1.ppm
Output file: overlay_100_test0_test1.ppm
```

# Fun fact
The comments in code are written in Romanian (sorry for that).
