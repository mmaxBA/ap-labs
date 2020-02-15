#!/usr/bin/env python3

def longestSubstring(word):

    longest = ""
    charactersSeen = []

    for i in range(0, len(word)):
        for char in word[i:]:
            if char not in charactersSeen:
                charactersSeen.append(char)
                continue
            else:
                long = "".join(charactersSeen)
                if len(long) > len(longest):
                    longest = long
                charactersSeen.clear()
                break

    return len(longest)

word = input()
print(longestSubstring(word))