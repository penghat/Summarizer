# Summarizer

Want to stay educated on current events, but don't necessarily have the time to read dozens of long articles every day? This program will read the text of an article and create a "summary" *(the most relevant sentences)* of that text. The user can specify the desired length *(number of lines)* of the summary.

# Use
The program takes the text file with the article and the summary length as command line arguments. Other methods of input are not currently accepted.

**Generic example:** *<a.out> <file_with_text.txt> <summary_length>*

The program will create a text file with the same name *(until **".txt"** as the input file with the text **"_summary.txt"** appended)*.

**Generic example:** "example.txt" will result in the summary being stored in a file called "example_summary.txt"

# Constraints
1. The entire text can have at most 3,000 unique words.

2. The entire text can have at most 100 unique sentences.

3. Each paragraph can have at most 5,000 characters.

These values are somewhat arbitrary. However, these constraints can be easily changed.
