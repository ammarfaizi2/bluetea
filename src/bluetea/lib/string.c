// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/lib/string.c
 *
 *  String library
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <bluetea/lib/string.h>
#include <stdio.h>


__no_inline char *strtrim(char *str)
{
	return strtriml(str, strlen(str));
}


__no_inline char *strtriml(char *str, size_t len)
{
	char *end;

	if (unlikely(len == 0))
		return str;


	/*
	 * We assume that `str + len` is the location of the NUL char
	 */
	end = str + len - 1;


	while (is_ws(*str)) {

		if (str == &end[1]) {
			*str = '\0';
			return str;
		}

		str++;
	}


	if (*str == '\0' && str == &end[1]) {
		/*
		 * All spaces C string, or empty C string will go here.
		 */
		return str;
	}


	while (is_ws(*end))
		end--;


	end[1] = '\0';
	return str;
}


__no_inline char *strtrim_move(char *str)
{
	return strtriml_move(str, strlen(str));
}


__no_inline char *strtriml_move(char *str, size_t len)
{
	size_t trimmed_len;
	char *orig = str, *end;

	if (unlikely(len == 0))
		return orig;


	/*
	 * We assume that `str + len` is the location of the NUL char
	 */
	end = str + len - 1;


	while (is_ws(*str)) {

		if (str == &end[1]) {
			*orig = '\0';
			return orig;
		}

		str++;
	}


	if (*str == '\0' && str == &end[1]) {
		/*
		 * All spaces C string, or empty C string will go here.
		 */
		*orig = '\0';
		return orig;
	}


	while (is_ws(*end))
		end--;


	trimmed_len = (size_t)(end - str) + 1u;
	if (orig != str)
		memmove(orig, str, trimmed_len);

	orig[trimmed_len] = '\0';
	return orig;
}


struct html_char_map {
	const char	to[8];
	const uint8_t	len;
};


static const struct html_char_map html_map[0x100u] = {
	['<']  = {"&lt;",	4},
	['>']  = {"&gt;",	4},
	['"']  = {"&quot;",	6},
	['\''] = {"&#039;",	6},
	['&']  = {"&amp;",	5},
};


__no_inline size_t htmlspecialchars(char *__restrict__ _out, size_t outlen,
				    const char *__restrict__ _in)
{
	unsigned char c;
	unsigned char *__restrict__ out = (unsigned char *)_out;
	const unsigned char *__restrict__ in  = (const unsigned char *)_in;
	const unsigned char *out_end = (const unsigned char *)(_out + outlen);

	if (unlikely(outlen == 0 || *in == '\0'))
		return 0;

	/*
	 * IMPORTANT:
	 * Don't alter `out_end[0]`, it is beyond the allowed area for writing.
	 */
	while ((c = *in)) {
		const struct html_char_map *map_to = &html_map[(size_t)c];

		if (likely(*map_to->to == '\0')) {
			/*
			 * We don't have this character on the map.
			 * Don't translate this character!
			 */
			if (unlikely(out + 1 >= out_end))
				/*
				 * We run out of buffer, don't copy!
				 */
				break;

			*out++ = c;
		} else {
			/*
			 * We find the corresponding character on the map.
			 * Translate this character!
			 */
			size_t len = map_to->len;
			if (unlikely(out + len >= out_end))
				/*
				 * We run out of buffer, don't copy!
				 */
				break;

			short_memcpy(out, map_to->to, (uint8_t)len);
			out += len;
		}
		in++;
	}

	*out = '\0';
	return (size_t)(out - (unsigned char *)_out);
}



__no_inline size_t htmlspecialcharsl(char *__restrict__ _out, size_t outlen,
				     const char *__restrict__ _in, size_t inlen)
{
	unsigned char *__restrict__ out = (unsigned char *)_out;
	const unsigned char *__restrict__ in  = (const unsigned char *)_in;
	const unsigned char *in_end  = (const unsigned char *)(_in + inlen);
	const unsigned char *out_end = (const unsigned char *)(_out + outlen);

	if (unlikely(outlen == 0 || inlen == 0))
		return 0;

	/*
	 * IMPORTANT:
	 * Don't alter `out_end[0]`, it is beyond the allowed area for writing.
	 */
	while (in < in_end) {
		unsigned char c = *in;
		const struct html_char_map *map_to = &html_map[(size_t)c];

		if (likely(*map_to->to == '\0')) {
			/*
			 * We don't have this character on the map.
			 * Don't translate this character!
			 */
			if (unlikely(out + 1 >= out_end))
				/*
				 * We run out of buffer, don't copy!
				 */
				break;

			*out++ = c;
		} else {
			/*
			 * We find the corresponding character on the map.
			 * Translate this character!
			 */
			size_t len = map_to->len;
			if (unlikely(out + len >= out_end))
				/*
				 * We run out of buffer, don't copy!
				 */
				break;

			short_memcpy(out, map_to->to, (uint8_t)len);
			out += len;
		}
		in++;
	}

	*out = '\0';
	return (size_t)(out - (unsigned char *)_out);
}
