/*
 * Copyright (c) 2020 Brian Callahan <bcallah@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freebee.h"
#include "sowpods.h"

static void
random_letters(void)
{
	int i, j;

	for (i = 0; i < sizeof(letters) - 1; i++) {
bad:
		letters[i] = (unsigned char) arc4random_uniform(26) + 'a';
		for (j = 0; j < i; j++) {
			if (letters[j] == letters[i])
				goto bad;
		}
	}
}

/*
 * The seventh letter is the center letter.
 * All valid words must have at least 1 occurance of it.
 */
void
create_anagrams(void)
{
	int pangram;
	int one, two, three, four, five, six;
	size_t i, j, total, word = 0, words, special, yes;

	/*
	 * Start by generating 7 distinct letters.
	 */
	(void) memset(letters, 0, sizeof(letters));

again:
	random_letters();

	/* Create anagrams.  */
	(void) memset(wordlist, 0, sizeof(wordlist));
	total = 0;
	pangram = 0;
	word = 0;
	words = 0;
	while (word < sizeof(sowpods) / sizeof(sowpods[0])) {
		j = 0;
		special = 0;
		yes = 0;
		one = two = three = four = five = six = 0;
		while (sowpods[word][j] != '\0') {
			for (i = 0; i < 7; i++) {
				if (sowpods[word][j] == letters[i]) {
					++yes;
					switch (i) {
					case 0:
						one = 1;
						break;
					case 1:
						two = 1;
						break;
					case 2:
						three = 1;
						break;
					case 3:
						four = 1;
						break;
					case 4:
						five = 1;
						break;
					case 5:
						six = 1;
						break;
					case 6:
						special = 1;
					}
					break;
				}
			}
			++j;
		}
		if (special && yes == strlen(sowpods[word])) {
			(void) strlcat(wordlist[words], sowpods[word], sizeof(wordlist[words]));
			++words;
			if (special && one && two && three && four && five && six) {
				pangram = 1;
				total += strlen(sowpods[word]) + 7;
			} else {
				if ((i = strlen(sowpods[word])) == 4)
					i = 1;
				total += i;
			}
		}
		++word;
	}

	/* Not a good game, try again.  */
	if (!pangram || words < 20 || words > 2000)
		goto again;

	puts("Status: 200 OK\r");
	puts("Content-Type: application/json\r");
	puts("\r");
	printf("{\n\t\"letters\": \"%s\",\n\t\"words\": %zu,\n\t\"total\": %zu,\n", letters, words, total);
	printf("\t\"wordlist\": [");
	for (i = 0; i < words; i++) {
		printf("\n\t\t\"%s\"", wordlist[i]);
		if (i != words - 1)
			putchar(',');
	}
	puts("\n\t]\n}");
}
