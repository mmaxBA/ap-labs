package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	var words []string
	var wordCount map[string]int
	
	words = strings.Fields(s)
	wordCount = make(map[string]int)
	
	for _,word := range words {
             wordCount[word] += 1 
        }

	return wordCount
}

func main() {
	wc.Test(WordCount)
}
