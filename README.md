# Compression
I tried my hand at a compression idea... this is what happened

#### The idea
  The idea came from a friend who is very interested in mathematics and who showed me that basic text ecoding could be done by sorting the characters by frequency and then assigning a place in a binary tree so that the more common the character is, the closer to the base it is. That way, you could make a tree starting with 1 and then each node having children that are either 1 or 0 so that the bitwise representation of each character can be constructed by going through the tree from the root to the location of that character.

#### The revision
  The first problem that I noticed was that there was no short, unique combination of bits that clearly indicates the end of the representation of a character, so I had to fix that. The idea that I came up with is to add 00 at the end of each character representation and to change the rules with which the tree is built. In order to make 00 unique, 1 can have 0 or 1 as children but 0 can only have 1, then each representation ends in either 00 or 000, creating a clear border between characters. In this way, an ascii character can be represented in 3 to 13 bits but, because frequency is taken into account, the average is lower than the usual 8.

#### The conclusion
  After some testing with files of various size (from short text to 60MB of text), the algorithm manages to losslessly compress files containing ascii characters and then decompress them with the only difference being that it adds a newline character at the end. For shorter files the compression is not that great since it needs to add the characters in order at the beginning, but for larger files the compression is around 66% of the original file size.
