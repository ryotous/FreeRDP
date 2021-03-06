/**
 * FreeRDP: A Remote Desktop Protocol Client
 * Glyph Cache
 *
 * Copyright 2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <freerdp/utils/stream.h>
#include <freerdp/utils/memory.h>

#include <freerdp/cache/glyph.h>

void* glyph_get(rdpGlyph* glyph, uint8 id, uint16 index, void** extra)
{
	void* entry;

	if (id > 9)
	{
		printf("invalid glyph cache id: %d\n", id);
		return NULL;
	}

	if (index > glyph->glyphCache[id].number)
	{
		printf("invalid glyph cache index: %d in cache id: %d\n", index, id);
		return NULL;
	}

	entry = glyph->glyphCache[id].entries[index].entry;

	if (extra != NULL)
		*extra = glyph->glyphCache[id].entries[index].extra;

	return entry;
}

void glyph_put(rdpGlyph* glyph, uint8 id, uint16 index, void* entry, void* extra)
{
	if (id > 9)
	{
		printf("invalid glyph cache id: %d\n", id);
		return;
	}

	if (index > glyph->glyphCache[id].number)
	{
		printf("invalid glyph cache index: %d in cache id: %d\n", index, id);
		return;
	}

	glyph->glyphCache[id].entries[index].entry = entry;

	if (extra != NULL)
		glyph->glyphCache[id].entries[index].extra = extra;
}

rdpGlyph* glyph_new(rdpSettings* settings)
{
	rdpGlyph* glyph;

	glyph = (rdpGlyph*) xzalloc(sizeof(rdpGlyph));

	if (glyph != NULL)
	{
		int i;

		glyph->settings = settings;

		//settings->glyphSupportLevel = GLYPH_SUPPORT_FULL;

		for (i = 0; i < 10; i++)
		{
			glyph->glyphCache[i].number = settings->glyphCache[i].cacheEntries;
			glyph->glyphCache[i].maxCellSize = settings->glyphCache[i].cacheMaximumCellSize;
			glyph->glyphCache[i].entries = xzalloc(sizeof(GLYPH_CACHE) * glyph->glyphCache[i].number);
		}

		glyph->fragCache.number = settings->fragCache.cacheEntries;
		glyph->fragCache.maxCellSize = settings->fragCache.cacheMaximumCellSize;
		glyph->fragCache.entries = xzalloc(sizeof(GLYPH_CACHE) * glyph->fragCache.number);
	}

	return glyph;
}

void glyph_free(rdpGlyph* glyph)
{
	if (glyph != NULL)
	{
		int i;

		for (i = 0; i < 10; i++)
		{
			xfree(glyph->glyphCache[i].entries);
		}

		xfree(glyph->fragCache.entries);

		xfree(glyph);
	}
}
