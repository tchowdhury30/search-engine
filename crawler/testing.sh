
# Testing with wrong number of arguments
echo "Testing with wrong number of arguments\n"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawldata/letters-0 5 huh
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"
# testing with out of range depth value
echo "testing with out of range depth value\n"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawldata/letters-0 150
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

# Testing with a non-existent link
echo "Testing with a non-existent link\n"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.htmlbruhbruhbruhbruh ../data/letters 5
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

# Testing with a non-existent directory
echo -e "testing with improper directory\n"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../berw/letters 5
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

# Testing with a non-0 depth
echo "Crawling letters-2, depth = 3; starting URL depth is 2"
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/B.html ../data/crawldata/letters-1 3

echo "Running valgrind on crawling letters-1, depth = 4"
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"
valgrind --leak-check=full --show-leak-kinds=all \
    ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawldata/letters-1 4

echo "Crawling letters-2, depth = 6"
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawldata/letters-2 5

echo "Crawling letters-22, depth = 3"
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawldata/letters-22 3

echo -e "Sample of two files in letters-22\n"
head -3 ../data/crawldata/letters-22/1
head -3 ../data/crawldata/letters-22/2
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

echo "Running Valgrind on toscrape-3, depth = 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/crawldata/toscrape-3 1
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

echo "Crawling wikipedia_2, depth = 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/crawldata/wikipedia_2 1
echo -e "Sample a file in wikipedia\n"
head -3 ../data/crawldata/wikipedia_2/6
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"