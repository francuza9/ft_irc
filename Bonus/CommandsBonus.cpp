#include "Bot.hpp"

std::string	Bot::translate(std::string line, std::string language)
{
	if (language == "Georgian")
		return (std::string(translateGeo(line)));
	else if (language == "Russian")
		return (std::string(translateRus(line)));
	return (line);
}

void	Bot::speak(int sock,std::string sender, std::string lang)
{
	if (lang == "GEO")
	{
		sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", sender);
		sendPrivmsg(sock, "ერთი უცნაური ზნე სჭირდა ბაგრატას: თავისი ეზოს\r\n", sender);
		sendPrivmsg(sock, "ყორეს ყოველ გაზაფხულზე ანგრევდა და ხელახლა აშენებდა.\r\n", sender);
		sendPrivmsg(sock, "უბნის ბიჭები პირველ წელიწადს შველოდნენ, მეორე\r\n", sender);
		sendPrivmsg(sock, "წელიწადსაც მიეხმარნენ, მაგრამ მესამე გაზაფხულზედაც\r\n", sender);
		sendPrivmsg(sock, "რომ შემოანგრია, ზედაც აღარ მიჰხედეს.\r\n", sender);
		sendPrivmsg(sock, "გოგლა ექვსი წლისა იყო, ბაგრატამ მეშვიდედ რომ შემოანგრია\r\n", sender);
		sendPrivmsg(sock, "თავის ეზოს ქვა-ტალახის ყორე, აურია ტალახი და დაიწყო აშენება.\r\n", sender);
		sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", sender);
	}
	else
	{
		sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", sender);
		sendPrivmsg(sock, "БЕСЕДЫ С ПРАВОСЛАВНЫМ ПСИХИАТРОМ. ВОПРОСЫ И ОТВЕТЫ.\r\n", sender);
		sendPrivmsg(sock, "СВЯТЫЕ ОТЦЫ О БОЛЕЗНЯХ И ЗДОРОВЬЕ.\r\n", sender);
		sendPrivmsg(sock, "Мы с Нико Наварро друзья.Его фирменная ухмылка и игривость\r\n", sender);
		sendPrivmsg(sock, "очаровательны, за исключением тех случаев, когда вы оказываетесь на стороне...\r\n", sender);
		sendPrivmsg(sock, "Эйвери Соко, ясноглазая и амбициозная доброжелательница,\r\n", sender);
		sendPrivmsg(sock, "находится на пороге получения своей мечты - работы по сбору средств в...\r\n", sender);
		sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", sender);
	}
}

void	Bot::hymn(int sock,std::string sender, std::string lang)
{
	if (lang == "GEO")
	{
		sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", sender);
		sendPrivmsg(sock, "ჩემი ხატია სამშობლო,\r\n", sender);
		sendPrivmsg(sock, "სახატე მთელი ქვეყანა,\r\n", sender);
		sendPrivmsg(sock, "განათებული მთა-ბარი,\r\n", sender);
		sendPrivmsg(sock, "წილნაყარია ღმერთთანა.\r\n", sender);
		sendPrivmsg(sock, "თავისუფლება დღეს ჩვენი\r\n", sender);
		sendPrivmsg(sock, "მომავალს უმღერს დიდებას,\r\n", sender);
		sendPrivmsg(sock, "ცისკრის ვარსკვლავი ამოდის\r\n", sender);
		sendPrivmsg(sock, "ამოდის და ორ ზღვას შუა ბრწყინდება,\r\n", sender);
		sendPrivmsg(sock, "დიდება თავისუფლებას,\r\n", sender);
		sendPrivmsg(sock, "თავისუფლებას დიდება!\r\n", sender);
		sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", sender);
	}
	else
	{
		sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", sender);
		sendPrivmsg(sock, "Россия — священная наша держава,\r\n", sender);
		sendPrivmsg(sock, "Россия — любимая наша страна.\r\n", sender);
		sendPrivmsg(sock, "Могучая воля, великая слава –\r\n", sender);
		sendPrivmsg(sock, "Твоё достоянье на все времена!\r\n", sender);
		sendPrivmsg(sock, "Славься, Отечество наше свободное,\r\n", sender);
		sendPrivmsg(sock, "Братских народов союз вековой,\r\n", sender);
		sendPrivmsg(sock, "Предками данная мудрость народная!\r\n", sender);
		sendPrivmsg(sock, "Славься, страна! Мы гордимся тобой!\r\n", sender);
		sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", sender);
	}
}

void	Bot::help(int sock, std::string recieved)
{
	sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", getSender(recieved));
	sendPrivmsg(sock, RPL_CMDS, getSender(recieved));
	sendPrivmsg(sock, RPL_SET, getSender(recieved));
	sendPrivmsg(sock, ERR_SET, getSender(recieved));
	sendPrivmsg(sock, RPL_TEACH, getSender(recieved));
	sendPrivmsg(sock, ERR_TEACH, getSender(recieved));
	sendPrivmsg(sock, RPL_SPEAK, getSender(recieved));
	sendPrivmsg(sock, ERR_SPEAK, getSender(recieved));
	sendPrivmsg(sock, RPL_HYMN, getSender(recieved));
	sendPrivmsg(sock, ERR_HYMN, getSender(recieved));
	sendPrivmsg(sock, RPL_QUIT, getSender(recieved));
	sendPrivmsg(sock, ERR_QUIT, getSender(recieved));
	sendPrivmsg(sock, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", getSender(recieved));
}
void	Bot::teach(int sock,std::string sender)
{
	sendPrivmsg(sock, "Georgian Lesson (Lazy to do Russian too :) )\r\n", sender);
	sendPrivmsg(sock, "Hello --> Gamarjoba --> გამარჯობა\r\n", sender);
	sendPrivmsg(sock, "Georgian Has 33 Letters in it's unique alphabet\r\n", sender);
	sendPrivmsg(sock, "Only 4 languages use this alphabet and all of them\r\n", sender);
	sendPrivmsg(sock, "are based in Georgia.\r\n", sender);
	sendPrivmsg(sock, "Enough for the boring stuff. fun facts\r\n", sender);
	sendPrivmsg(sock, "Oldest wine in the world.\r\n", sender);
	sendPrivmsg(sock, "First ever \"European\" people were found in georgia\r\n", sender);
	sendPrivmsg(sock, "Stalin was Georgian\r\n", sender);
	sendPrivmsg(sock, "We are 3 million in the world\r\n", sender);
	sendPrivmsg(sock, "We have the biggest wendy's in the world for some reason.\r\n", sender);
	sendPrivmsg(sock, "THE END\r\n", sender);
}

void	Bot::quit(int fd)
{
	sendMessage(fd, "QUIT :Because you asked me to.\r\n");
	close(fd);
}

void	Bot::inviteAccept(int fd, std::string channel)
{
	sendMessage(fd, "JOIN " + channel + "\r\n");
}
