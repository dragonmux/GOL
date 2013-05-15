/*
 * This file is part of GOL
 * Copyright © 2013 Rachel Mant (dx-mon@users.sourceforge.net)
 *
 * GOL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GOL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

void diamondRing(uint32_t x, uint32_t y)
{
	set(x + 6, y + 0, true);
	set(x + 5, y + 1, true);
	set(x + 7, y + 1, true);
	set(x + 4, y + 2, true);
	set(x + 6, y + 2, true);
	set(x + 8, y + 2, true);
	set(x + 4, y + 3, true);
	set(x + 8, y + 3, true);
	set(x + 2, y + 4, true);
	set(x + 3, y + 4, true);
	set(x + 6, y + 4, true);
	set(x + 9, y + 4, true);
	set(x + 10, y + 4, true);
	set(x + 1, y + 5, true);
	set(x + 6, y + 5, true);
	set(x + 11, y + 5, true);
	set(x + 0, y + 6, true);
	set(x + 2, y + 6, true);
	set(x + 4, y + 6, true);
	set(x + 5, y + 6, true);
	set(x + 7, y + 6, true);
	set(x + 8, y + 6, true);
	set(x + 10, y + 6, true);
	set(x + 12, y + 6, true);
	set(x + 1, y + 7, true);
	set(x + 6, y + 7, true);
	set(x + 11, y + 7, true);
	set(x + 2, y + 8, true);
	set(x + 3, y + 8, true);
	set(x + 6, y + 8, true);
	set(x + 9, y + 8, true);
	set(x + 10, y + 8, true);
	set(x + 4, y + 9, true);
	set(x + 8, y + 9, true);
	set(x + 4, y + 10, true);
	set(x + 6, y + 10, true);
	set(x + 8, y + 10, true);
	set(x + 5, y + 11, true);
	set(x + 7, y + 11, true);
	set(x + 6, y + 12, true);
}
