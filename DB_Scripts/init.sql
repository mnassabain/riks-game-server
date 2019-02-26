--users
CREATE TABLE users(
login varchar(60),
password varchar(200) NOT NULL,
constraint pk_users PRIMARY KEY (login)
);

--users' statistics
CREATE TABLE stats(
login varchar(60) UNIQUE NOT NULL,
victories int(11) NOT NULL,
defeats int(11) NOT NULL,
gained_territories int(11) NOT NULL,
lost_territories int(11) NOT NULL,
reinforcement int(11) NOT NULL,
sets_of_tokens int(11) NOT NULL,
CONSTRAINT fk_stats_users FOREIGN KEY(login) references users(login) on delete cascade
);

--games ranking
CREATE TABLE ranking(
gameid int(11) NOT NULL AUTO_INCREMENT,
players int(11) NOT NULL,
firstp varchar(60) NOT NULL,
secondp varchar(60) NOT NULL,
thirdp varchar(60),
fourthp varchar(60),
fifthp varchar(60),
sixthp varchar(60),
CONSTRAINT pk_ranking PRIMARY KEY (gameid),
CONSTRAINT ck_players CHECK (players >1 and players <7),
CONSTRAINT fk_first_users FOREIGN KEY(firstp) references users(login) on delete cascade,
CONSTRAINT fk_second_users FOREIGN KEY(secondp) references users(login) on delete cascade,
CONSTRAINT fk_third_users FOREIGN KEY(thirdp) references users(login) on delete cascade,
CONSTRAINT fk_fourth_users FOREIGN KEY(fourthp) references users(login) on delete cascade,
CONSTRAINT fk_fifth_users FOREIGN KEY(fifthp) references users(login) on delete cascade,
CONSTRAINT fk_sixth_users FOREIGN KEY(sixthp) references users(login) on delete cascade
);