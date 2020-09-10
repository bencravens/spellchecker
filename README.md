# 242assignment
## TODO: 
- [x] When finished running through filename.txt, print the timing information and unknown word count to stderr like this: https://imgur.com/MB35ceI.png
- [x] add the following command line arguments to the script: (c, d, f, o, r, h) https://imgur.com/BZG1Ain.png
- [x] c (spellcheck)
- [x] d (print depth of tree)
- [x] f (dot output to custom filename)
- [x] o (dot output)
- [x] r (initialize tree as rbt)
- [x] h (help message)
- [ ] test different combinations of command line arguments i.e 
```bash
./output < smalldictionary.txt -r -c test.txt -f mydot.dot
```
- [ ] comment everything and tidy it up
- [ ] test it using the script supplied by comp sci department
example of this: (must test for many different outputs)
```bash
./output < smalldictionary.txt -r -c test.txt >> ouroutput.txt
./sample-asgn < smalldictionary.txt -r -c test.txt >> modeloutput.txt
```
- [ ] make it so that it is agnostic to properly capitalized words, i.e it spellchecks "Test" as "test" but not "tEst" as "test". The checker script does this so we need to do it too.
