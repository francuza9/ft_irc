#include "Bot.hpp"

std::string	Bot::translateGeo(std::string line)
{
	for (size_t i = 0; line[i]; i++)
	{
		if (line[i] == 'a')
			line.replace(i, 1, "ა");
		else if (line[i] == 'b')
			line.replace(i, 1, "ბ");
		else if (line[i] == 'g')
			line.replace(i, 1, "გ");
		else if (line[i] == 'd')
			line.replace(i, 1, "დ");
		else if (line[i] == 'e')
			line.replace(i, 1, "ე");
		else if (line[i] == 'v')
			line.replace(i, 1, "ვ");
		else if (line[i] == 'z')
			line.replace(i, 1, "ზ");
		else if (line[i] == 'T')
			line.replace(i, 1, "თ");
		else if (line[i] == 'i')
			line.replace(i, 1, "ი");
		else if (line[i] == 'k')
			line.replace(i, 1, "კ");
		else if (line[i] == 'l')
			line.replace(i, 1, "ლ");
		else if (line[i] == 'm')
			line.replace(i, 1, "მ");
		else if (line[i] == 'n')
			line.replace(i, 1, "ნ");
		else if (line[i] == 'o')
			line.replace(i, 1, "ო");
		else if (line[i] == 'p')
			line.replace(i, 1, "პ");
		else if (line[i] == 'J')
			line.replace(i, 1, "ჟ");
		else if (line[i] == 'r')
			line.replace(i, 1, "რ");
		else if (line[i] == 's')
			line.replace(i, 1, "ს");
		else if (line[i] == 't')
			line.replace(i, 1, "ტ");
		else if (line[i] == 'u')
			line.replace(i, 1, "უ");
		else if (line[i] == 'f')
			line.replace(i, 1, "ფ");
		else if (line[i] == 'q')
			line.replace(i, 1, "ქ");
		else if (line[i] == 'R')
			line.replace(i, 1, "ღ");
		else if (line[i] == 'y')
			line.replace(i, 1, "ყ");
		else if (line[i] == 'S')
			line.replace(i, 1, "შ");
		else if (line[i] == 'C')
			line.replace(i, 1, "ჩ");
		else if (line[i] == 'c')
			line.replace(i, 1, "ც");
		else if (line[i] == 'Z')
			line.replace(i, 1, "ძ");
		else if (line[i] == 'w')
			line.replace(i, 1, "წ");
		else if (line[i] == 'W')
			line.replace(i, 1, "ჭ");
		else if (line[i] == 'x')
			line.replace(i, 1, "ხ");
		else if (line[i] == 'j')
			line.replace(i, 1, "ჯ");
		else if (line[i] == 'h')
			line.replace(i, 1, "ჰ");
	}
	return (line);
}

std::string	Bot::translateRus(std::string line)
{
	for (size_t i = 0; line[i]; i++)
	{
		if (line[i] == 'a')
			line.replace(i, 1, "а");
		else if (line[i] == 'A')
			line.replace(i, 1, "я");
		else if (line[i] == 'b')
			line.replace(i, 1, "б");
		else if (line[i] == 'B')
			line.replace(i, 1, "ь");
		else if (line[i] == 'g')
			line.replace(i, 1, "г");
		else if (line[i] == 'd')
			line.replace(i, 1, "д");
		else if (line[i] == 'e')
			line.replace(i, 1, "е");
		else if (line[i] == 'E')
			line.replace(i, 1, "э");
		else if (line[i] == 'v')
			line.replace(i, 1, "в");
		else if (line[i] == 'z')
			line.replace(i, 1, "з");
		else if (line[i] == 't')
			line.replace(i, 1, "т");
		else if (line[i] == 'T')
			line.replace(i, 1, "ъ");
		else if (line[i] == 'i')
			line.replace(i, 1, "и");
		else if (line[i] == 'I')
			line.replace(i, 1, "ы");
		else if (line[i] == 'k')
			line.replace(i, 1, "к");
		else if (line[i] == 'l')
			line.replace(i, 1, "л");
		else if (line[i] == 'm')
			line.replace(i, 1, "м");
		else if (line[i] == 'n')
			line.replace(i, 1, "н");
		else if (line[i] == 'o')
			line.replace(i, 1, "о");
		else if (line[i] == 'p')
			line.replace(i, 1, "п");
		else if (line[i] == 'j')
			line.replace(i, 1, "ж");
		else if (line[i] == 'r')
			line.replace(i, 1, "р");
		else if (line[i] == 's')
			line.replace(i, 1, "с");
		else if (line[i] == 'u')
			line.replace(i, 1, "у");
		else if (line[i] == 'U')
			line.replace(i, 1, "ю");
		else if (line[i] == 'f')
			line.replace(i, 1, "ф");
		else if (line[i] == 'c')
			line.replace(i, 1, "ц");
		else if (line[i] == 'x')
			line.replace(i, 1, "х");
		else if (line[i] == 'h')
			line.replace(i, 1, "й");
		else if (line[i] == 'C')
			line.replace(i, 1, "ч");
		else if (line[i] == 'H')
			line.replace(i, 1, "ш");
	}
	return (line);
}