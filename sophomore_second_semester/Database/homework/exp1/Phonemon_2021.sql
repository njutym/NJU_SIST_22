/* prepare */

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema phonemon
-- UNCOMMENT THE BELOW 3x LINES IF YOU RUN THE SCRIPT ON YOUR OWN LOCAL SERVER
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `phonemon` ;
CREATE SCHEMA IF NOT EXISTS `phonemon` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `phonemon` ;

-- -----------------------------------------------------
-- Table `Type`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Type`;
CREATE TABLE IF NOT EXISTS `Type` (
  `id` TINYINT NOT NULL,
  `title` VARCHAR(50) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Species`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Species`;
CREATE TABLE IF NOT EXISTS `Species` (
  `id` TINYINT UNSIGNED NOT NULL,
  `title` VARCHAR(50) NOT NULL,
  `type1` TINYINT NOT NULL,
  `type2` TINYINT NULL,
  `description` VARCHAR(300) NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Species_Type1_idx` (`type1` ASC),
  CONSTRAINT `fk_Species_Type1`
    FOREIGN KEY (`type1`)
    REFERENCES `Type` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Species_Type2`
    FOREIGN KEY (`type2`)
    REFERENCES `Type` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Team`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Team`;
CREATE TABLE IF NOT EXISTS `Team` (
  `id` CHAR(1) NOT NULL,
  `title` VARCHAR(50) NULL,
  `colour` CHAR(10) NULL,
  `leader` CHAR(10) NULL,
  `mascot` TINYINT UNSIGNED NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Team_Species1_idx` (`mascot` ASC),
  CONSTRAINT `fk_Team_Species1`
    FOREIGN KEY (`mascot`)
    REFERENCES `Species` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Player`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Player`;
CREATE TABLE IF NOT EXISTS `Player` (
  `id` INT NOT NULL,
  `username` VARCHAR(50) NOT NULL,
  `joined` TIMESTAMP NOT NULL,
  `latitude` DECIMAL(8,6) NULL,
  `longitude` DECIMAL(9,6) NULL,
  `level` TINYINT NOT NULL,
  `team` CHAR(1) NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Player_Team1_idx` (`team` ASC),
  CONSTRAINT `fk_Player_Team1`
    FOREIGN KEY (`team`)
    REFERENCES `Team` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Phonemon`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Phonemon`;
CREATE TABLE IF NOT EXISTS `Phonemon` (
  `id` BIGINT NOT NULL,
  `species` TINYINT UNSIGNED NOT NULL,
  `player` INT NULL,
  `whenSpawned` TIMESTAMP NOT NULL,
  `latitude` DECIMAL(8,6) NOT NULL,
  `longitude` DECIMAL(9,6) NOT NULL,
  `power` SMALLINT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Pokemon_Player_idx` (`player` ASC),
  INDEX `fk_Pokemon_Species1_idx` (`species` ASC),
  CONSTRAINT `fk_Pokemon_Player`
    FOREIGN KEY (`player`)
    REFERENCES `Player` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Pokemon_Species1`
    FOREIGN KEY (`species`)
    REFERENCES `Species` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Item`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Item`;
CREATE TABLE IF NOT EXISTS `Item` (
  `id` TINYINT NOT NULL,
  `type` CHAR NULL,
  `title` VARCHAR(50) NOT NULL,
  `price` DECIMAL(4,2) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Purchase`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Purchase`;
CREATE TABLE IF NOT EXISTS `Purchase` (
  `id` BIGINT NOT NULL,
  `player` INT NOT NULL,
  `item` TINYINT NOT NULL,
  `quantity` TINYINT NOT NULL,
  `whenPurchased` TIMESTAMP NOT NULL,
  PRIMARY KEY (`id`, `player`, `item`),
  INDEX `fk_Purchase_Player1_idx` (`player` ASC),
  INDEX `fk_Purchase_Item1_idx` (`item` ASC),
  CONSTRAINT `fk_Player_Player1` 
  FOREIGN KEY (`player`) REFERENCES `Player` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Purchase_Item1` 
  FOREIGN KEY (`item`) REFERENCES `Item` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Medicine`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Medicine`;
CREATE TABLE IF NOT EXISTS `Medicine` (
  `id` TINYINT NOT NULL,
  `points` SMALLINT NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_Medicine_Item1`
    FOREIGN KEY (`id`)
    REFERENCES `Item` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Food`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Food`;
CREATE TABLE IF NOT EXISTS `Food` (
  `id` TINYINT NOT NULL,
  `points` SMALLINT NOT NULL,
  `manna` SMALLINT NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_Food_Item1`
    FOREIGN KEY (`id`)
    REFERENCES `Item` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


/* now insert date in tables */

insert into Team values ('I','Instinct','yellow','Spark',145);
insert into Team values ('M','Mystic','blue','Blanche',144);
insert into Team values ('V','Valor','red','Candela',146);

insert into Type values (1,'Normal');
insert into Type values (2,'Fighting');
insert into Type values (3,'Flying');
insert into Type values (4,'Poison');
insert into Type values (5,'Ground');
insert into Type values (6,'Rock');
insert into Type values (7,'Bug');
insert into Type values (8,'Ghost');
insert into Type values (9,'Steel');
insert into Type values (10,'Fire');
insert into Type values (11,'Water');
insert into Type values (12,'Grass');
insert into Type values (13,'Electric');
insert into Type values (14,'Psychic');
insert into Type values (15,'Ice');
insert into Type values (16,'Dragon');
insert into Type values (17,'Dark');
insert into Type values (18,'Fairy');

insert into Species values (1,'Bulbasaur',12,4,'Bulbasaur can be seen napping in bright sunlight. There is a seed on its back. By soaking up the sun\'s rays, the seed grows progressively larger.');
insert into Species values (2,'Ivysaur',12,4,'There is a bud on this Phonemon\'s back. To support its weight, Ivysaur\'s legs and trunk grow thick and strong. If it starts spending more time lying in the sunlight, it\'s a sign that the bud will bloom into a large flower soon.');
insert into Species values (3,'Venusaur',12,4,'There is a large flower on Venusaur\'s back. The flower is said to take on vivid colors if it gets plenty of nutrition and sunlight. The flower\'s aroma soothes the emotions of people.');
insert into Species values (4,'Charmander',10,null,'The flame that burns at the tip of its tail is an indication of its emotions. The flame wavers when Charmander is enjoying itself. If the Phonemon becomes enraged, the flame burns fiercely.');
insert into Species values (5,'Charmeleon',10,null,'Charmeleon mercilessly destroys its foes using its sharp claws. If it encounters a strong foe, it turns aggressive. In this excited state, the flame at the tip of its tail flares with a bluish white color.');
insert into Species values (6,'Charizard',10,3,'Charizard flies around the sky in search of powerful opponents. It breathes fire of such great heat that it melts anything. However, it never turns its fiery breath on any opponent weaker than itself.');
insert into Species values (7,'Squirtle',11,null,'Squirtle\'s shell is not merely used for protection. The shell\'s rounded shape and the grooves on its surface help minimize resistance in water, enabling this Phonemon to swim at high speeds.');
insert into Species values (8,'Wartortle',11,null,'Its tail is large and covered with a rich, thick fur. The tail becomes increasingly deeper in color as Wartortle ages. The scratches on its shell are evidence of this Phonemon\'s toughness as a battler.');
insert into Species values (9,'Blastoise',11,null,'Blastoise has water spouts that protrude from its shell. The water spouts are very accurate. They can shoot bullets of water with enough accuracy to strike empty cans from a distance of over 160 feet.');
insert into Species values (10,'Caterpie',7,null,'Caterpie has a voracious appetite. It can devour leaves bigger than its body right before your eyes. From its antenna, this Phonemon releases a terrifically strong odor.');
insert into Species values (11,'Metapod',7,null,'The shell covering this Phonemon’s body is as hard as an iron slab. Metapod does not move very much. It stays still because it is preparing its soft innards for evolution inside the hard shell.');
-- insert into Species values (12,'Butterfree',7,3,'Butterfree has a superior ability to search for delicious honey from flowers. It can even search out, extract, and carry honey from flowers that are blooming over six miles from its nest.');
-- insert into Species values (13,'Weedle',7,4,'Weedle has an extremely acute sense of smell. It is capable of distinguishing its favorite kinds of leaves from those it dislikes just by sniffing with its big red proboscis (nose).');
-- insert into Species values (14,'Kakuna',7,4,'Kakuna remains virtually immobile as it clings to a tree. However, on the inside, it is extremely busy as it prepares for its coming evolution. This is evident from how hot the shell becomes to the touch.');
-- insert into Species values (15,'Beedrill',7,4,'Beedrill is extremely territorial. No one should ever approach its nest—this is for their own safety. If angered, they will attack in a furious swarm.');
insert into Species values (16,'Pidgey',1,3,'Pidgey has an extremely sharp sense of direction. It is capable of unerringly returning home to its nest, however far it may be removed from its familiar surroundings.');
insert into Species values (17,'Pidgeotto',1,3,'Pidgeotto claims a large area as its own territory. This Phonemon flies around, patrolling its living space. If its territory is violated, it shows no mercy in thoroughly punishing the foe with its sharp claws.');
insert into Species values (18,'Pidgeot',1,3,'This Phonemon has a dazzling plumage of beautifully glossy feathers. Many Trainers are captivated by the striking beauty of the feathers on its head, compelling them to choose Pidgeot as their Phonemon.');
insert into Species values (19,'Rattata',1,null,'Rattata is cautious in the extreme. Even while it is asleep, it constantly listens by moving its ears around. It is not picky about where it lives—it will make its nest anywhere.');
insert into Species values (20,'Raticate',1,null,'Raticate’s sturdy fangs grow steadily. To keep them ground down, it gnaws on rocks and logs. It may even chew on the walls of houses.');
insert into Species values (21,'Spearow',1,3,'Spearow has a very loud cry that can be heard over half a mile away. If its high, keening cry is heard echoing all around, it is a sign that they are warning each other of danger.');
insert into Species values (22,'Fearow',1,3,'Fearow is recognized by its long neck and elongated beak. They are conveniently shaped for catching prey in soil or water. It deftly moves its long and skinny beak to pluck prey.');
insert into Species values (23,'Ekans',4,null,'Ekans curls itself up in a spiral while it rests. Assuming this position allows it to quickly respond to a threat from any direction with a glare from its upraised head.');
insert into Species values (24,'Arbok',4,null,'This Phonemon is terrifically strong in order to constrict things with its body. It can even flatten steel oil drums. Once Arbok wraps its body around its foe, escaping its crunching embrace is impossible.');
insert into Species values (25,'Pikachu',13,null,'Whenever Pikachu comes across something new, it blasts it with a jolt of electricity. If you come across a blackened berry, it\'s evidence that this Phonemon mistook the intensity of its charge.');
insert into Species values (26,'Raichu',13,null,'If the electrical sacs become excessively charged, Raichu plants its tail in the ground and discharges. Scorched patches of ground will be found near this Phonemon\'s nest.');
insert into Species values (27,'Sandshrew',5,null,'Sandshrew\'s body is configured to absorb water without waste, enabling it to survive in an arid desert. This Phonemon curls up to protect itself from its enemies.');
insert into Species values (28,'Sandslash',5,null,'Sandslash\'s body is covered by tough spikes, which are hardened sections of its hide. Once a year, the old spikes fall out, to be replaced with new spikes that grow out from beneath the old ones.');
insert into Species values (29,'Nidoran-f',4,null,'Nidoran♀ has barbs that secrete a powerful poison. They are thought to have developed as protection for this small-bodied Phonemon. When enraged, it releases a horrible toxin from its horn.');
insert into Species values (30,'Nidorina',4,null,'When Nidorina are with their friends or family, they keep their barbs tucked away to prevent hurting each other. This Phonemon appears to become nervous if separated from the others.');
insert into Species values (31,'Nidoqueen',4,5,'Nidoqueen\'s body is encased in extremely hard scales. It is adept at sending foes flying with harsh tackles. This Phonemon is at its strongest when it is defending its young.');
insert into Species values (32,'Nidoran-m',4,null,'Nidoran♂ has developed muscles for moving its ears. Thanks to them, the ears can be freely moved in any direction. Even the slightest sound does not escape this Phonemon\'s notice.');
insert into Species values (33,'Nidorino',4,null,'Nidorino has a horn that is harder than a diamond. If it senses a hostile presence, all the barbs on its back bristle up at once, and it challenges the foe with all its might.');
insert into Species values (34,'Nidoking',4,5,'Nidoking\'s thick tail packs enormously destructive power. With one swing, it can topple a metal transmission tower. Once this Phonemon goes on a rampage, there is no stopping it.');
insert into Species values (35,'Clefairy',18,null,'On every night of a full moon, groups of this Phonemon come out to play. When dawn arrives, the tired Clefairy return to their quiet mountain retreats and go to sleep nestled up against each other.');
-- insert into Species values (36,'Clefable',18,null,'Clefable moves by skipping lightly as if it were flying using its wings. Its bouncy step even lets it walk on water. It is known to take strolls on lakes on quiet, moonlit nights.');
insert into Species values (37,'Vulpix',10,null,'At the time of its birth, Vulpix has one white tail. The tail separates into six if this Phonemon receives plenty of love from its trainer. The six tails become magnificently curled.');
insert into Species values (38,'Ninetales',10,null,'Ninetales casts a sinister light from its bright red eyes to gain total control over its foe\'s mind. This Phonemon is said to live for one thousand years.');
-- insert into Species values (39,'Jigglypuff',1,18,'Jigglypuff\'s vocal cords can freely adjust the wavelength of its voice. This Phonemon uses this ability to sing at precisely the right wavelength to make its foes most drowsy.');
-- insert into Species values (40,'Wigglytuff',1,18,'Wigglytuff has large, saucerlike eyes. The surfaces of its eyes are always covered with a thin layer of tears. If any dust gets in this Phonemon\'s eyes, it is quickly washed away.');
insert into Species values (41,'Zubat',4,3,'Zubat remains quietly unmoving in a dark spot during the bright daylight hours. It does so because prolonged exposure to the sun causes its body to become slightly burned.');
insert into Species values (42,'Golbat',4,3,'Golbat loves to drink the blood of living things. It is particularly active in the pitch black of night. This Phonemon flits around in the night skies, seeking fresh blood.');
insert into Species values (43,'Oddish',12,4,'During the daytime, Oddish buries itself in soil to absorb nutrients from the ground using its entire body. The more fertile the soil, the glossier its leaves become.');
insert into Species values (44,'Gloom',12,4,'Gloom releases a foul fragrance from the pistil of its flower. When faced with danger, the stench worsens. If this Phonemon is feeling calm and secure, it does not release its usual stinky aroma.');
insert into Species values (45,'Vileplume',12,4,'Vileplume\'s toxic pollen triggers atrocious allergy attacks. That\'s why it is advisable never to approach any attractive flowers in a jungle, however pretty they may be.');
-- insert into Species values (46,'Paras',7,12,'Paras has parasitic mushrooms growing on its back called tochukaso. They grow large by drawing nutrients from this Bug Phonemon host. They are highly valued as a medicine for extending life.');
-- insert into Species values (47,'Parasect',7,12,'Parasect is known to infest large trees en masse and drain nutrients from the lower trunk and roots. When an infested tree dies, they move onto another tree all at once.');
-- insert into Species values (48,'Venonat',7,4,'Venonat is said to have evolved with a coat of thin, stiff hair that covers its entire body for protection. It possesses large eyes that never fail to spot even miniscule prey.');
-- insert into Species values (49,'Venomoth',7,4,'Venomoth is nocturnal—it is a Phonemon that only becomes active at night. Its favorite prey are small insects that gather around streetlights, attracted by the light in the darkness.');
insert into Species values (50,'Diglett',5,null,'Diglett are raised in most farms. The reason is simple—wherever this Phonemon burrows, the soil is left perfectly tilled for planting crops. This soil is made ideal for growing delicious vegetables.');
insert into Species values (51,'Dugtrio',5,null,'Dugtrio are actually triplets that emerged from one body. As a result, each triplet thinks exactly like the other two triplets. They work cooperatively to burrow endlessly.');
insert into Species values (52,'Meowth',1,17,'Meowth withdraws its sharp claws into its paws to slinkily sneak about without making any incriminating footsteps. For some reason, this Phonemon loves shiny coins that glitter with light.');
insert into Species values (53,'Persian',1,null,'Persian has six bold whiskers that give it a look of toughness. The whiskers sense air movements to determine what is in the Phonemon\'s surrounding vicinity. It becomes docile if grabbed by the whiskers.');
insert into Species values (54,'Psyduck',11,null,'Psyduck uses a mysterious power. When it does so, this Phonemon generates brain waves that are supposedly only seen in sleepers. This discovery spurred controversy among scholars.');
insert into Species values (55,'Golduck',11,null,'The webbed flippers on its forelegs and hind legs and the streamlined body of Golduck give it frightening speed. The Phonemon is definitely much faster than even the most athletic swimmer.');
insert into Species values (56,'Mankey',2,null,'When Mankey starts shaking and its nasal breathing turns rough, it\'s a sure sign that it is becoming angry. However, because it goes into a towering rage almost instantly, it is impossible for anyone to flee its wrath.');
insert into Species values (57,'Primeape',2,null,'When Primeape becomes furious, its blood circulation is boosted. In turn, its muscles are made even stronger. However, it also becomes much less intelligent at the same time.');
insert into Species values (58,'Growlithe',10,null,'Growlithe has a superb sense of smell. Once it smells anything, this Phonemon won\'t forget the scent, no matter what. It uses its advanced olfactory sense to determine the emotions of other living things.');
insert into Species values (59,'Arcanine',10,null,'Arcanine is known for its high speed. It is said to be capable of running over 6,200 miles in a single day and night. The fire that blazes wildly within this Phonemon\'s body is its source of power.');
insert into Species values (60,'Poliwag',11,null,'Poliwag has a very thin skin. It is possible to see the Phonemon\'s spiral innards right through the skin. Despite its thinness, however, the skin is also very flexible. Even sharp fangs bounce right off it.');
insert into Species values (61,'Poliwhirl',11,null,'The surface of Poliwhirl\'s body is always wet and slick with an oily fluid. Because of this greasy covering, it can easily slip and slide out of the clutches of any enemy in battle.');
insert into Species values (62,'Poliwrath',11,2,'Poliwrath\'s highly developed, brawny muscles never grow fatigued, however much it exercises. It is so tirelessly strong, this Phonemon can swim back and forth across the ocean without effort.');
insert into Species values (63,'Abra',14,null,'Abra sleeps for eighteen hours a day. However, it can sense the presence of foes even while it is sleeping. In such a situation, this Phonemon immediately teleports to safety.');
insert into Species values (64,'Kadabra',14,null,'Kadabra emits a peculiar alpha wave if it develops a headache. Only those people with a particularly strong psyche can hope to become a trainer of this Phonemon.');
insert into Species values (65,'Alakazam',14,null,'Alakazam\'s brain continually grows, making its head far too heavy to support with its neck. This Phonemon holds its head up using its psychokinetic power instead.');
insert into Species values (66,'Machop',2,null,'Machop\'s muscles are special—they never get sore no matter how much they are used in exercise. This Phonemon has sufficient power to hurl a hundred adult humans.');
insert into Species values (67,'Machoke',2,null,'Machoke\'s thoroughly toned muscles possess the hardness of steel. This Phonemon has so much strength, it can easily hold aloft a sumo wrestler on just one finger.');
insert into Species values (68,'Machamp',2,null,'Machamp has the power to hurl anything aside. However, trying to do any work requiring care and dexterity causes its arms to get tangled. This Phonemon tends to leap into action before it thinks.');
insert into Species values (69,'Bellsprout',12,4,'Bellsprout\'s thin and flexible body lets it bend and sway to avoid any attack, however strong it may be. From its mouth, this Phonemon spits a corrosive fluid that melts even iron.');
insert into Species values (70,'Weepinbell',12,4,'Weepinbell has a large hook on its rear end. At night, the Phonemon hooks on to a tree branch and goes to sleep. If it moves around in its sleep, it may wake up to find itself on the ground.');
insert into Species values (71,'Victreebel',12,4,'Victreebel has a long vine that extends from its head. This vine is waved and flicked about as if it were an animal to attract prey. When an unsuspecting prey draws near, this Phonemon swallows it whole.');
insert into Species values (72,'Tentacool',11,4,'Tentacool\'s body is largely composed of water. If it is removed from the sea, it dries up like parchment. If this Phonemon happens to become dehydrated, put it back into the sea.');
insert into Species values (73,'Tentacruel',11,4,'Tentacruel has large red orbs on its head. The orbs glow before lashing the vicinity with a harsh ultrasonic blast. This Phonemon\'s outburst creates rough waves around it.');
insert into Species values (74,'Geodude',6,5,'The longer a Geodude lives, the more its edges are chipped and worn away, making it more rounded in appearance. However, this Phonemon\'s heart will remain hard, craggy, and rough always.');
insert into Species values (75,'Graveler',6,5,'Graveler grows by feeding on rocks. Apparently, it prefers to eat rocks that are covered in moss. This Phonemon eats its way through a ton of rocks on a daily basis.');
insert into Species values (76,'Golem',6,5,'Golem live up on mountains. If there is a large earthquake, these Phonemon will come rolling down off the mountains en masse to the foothills below.');
insert into Species values (77,'Ponyta',10,null,'Ponyta is very weak at birth. It can barely stand up. This Phonemon becomes stronger by stumbling and falling to keep up with its parent.');
insert into Species values (78,'Rapidash',10,null,'Rapidash usually can be seen casually cantering in the fields and plains. However, when this Phonemon turns serious, its fiery manes flare and blaze as it gallops its way up to 150 mph.');
insert into Species values (79,'Slowpoke',11,14,'Slowpoke uses its tail to catch prey by dipping it in water at the side of a river. However, this Phonemon often forgets what it\'s doing and often spends entire days just loafing at water\'s edge.');
insert into Species values (80,'Slowbro',11,14,'Slowbro\'s tail has a Shellder firmly attached with a bite. As a result, the tail can\'t be used for fishing anymore. This causes Slowbro to grudgingly swim and catch prey instead.');
insert into Species values (81,'Magnemite',13,9,'Magnemite attaches itself to power lines to feed on electricity. If your house has a power outage, check your circuit breakers. You may find a large number of this Phonemon clinging to the breaker box.');
insert into Species values (82,'Magneton',13,9,'Magneton emits a powerful magnetic force that is fatal to mechanical devices. As a result, large cities sound sirens to warn citizens of large-scale outbreaks of this Phonemon.');
insert into Species values (83,'Farfetchd',1,3,'Farfetch\'d is always seen with a stalk from a plant of some sort. Apparently, there are good stalks and bad stalks. This Phonemon has been known to fight with others over stalks.');
insert into Species values (84,'Doduo',1,3,'Doduo\'s two heads never sleep at the same time. Its two heads take turns sleeping, so one head can always keep watch for enemies while the other one sleeps.');
insert into Species values (85,'Dodrio',1,3,'Watch out if Dodrio\'s three heads are looking in three separate directions. It\'s a sure sign that it is on its guard. Don\'t go near this Phonemon if it\'s being wary-it may decide to peck you.');
insert into Species values (86,'Seel',11,null,'Seel hunts for prey in the frigid sea underneath sheets of ice. When it needs to breathe, it punches a hole through the ice with the sharply protruding section of its head.');
insert into Species values (87,'Dewgong',11,15,'Dewgong loves to snooze on bitterly cold ice. The sight of this Phonemon sleeping on a glacier was mistakenly thought to be a mermaid by a mariner long ago.');
insert into Species values (88,'Grimer',4,null,'Grimer\'s sludgy and rubbery body can be forced through any opening, however small it may be. This Phonemon enters sewer pipes to drink filthy wastewater.');
insert into Species values (89,'Muk',4,null,'From Muk\'s body seeps a foul fluid that gives off a nose-bendingly horrible stench. Just one drop of this Phonemon\'s body fluid can turn a pool stagnant and rancid.');
insert into Species values (90,'Shellder',11,null,'At night, this Phonemon uses its broad tongue to burrow a hole in the seafloor sand and then sleep in it. While it is sleeping, Shellder closes its shell, but leaves its tongue hanging out.');
insert into Species values (91,'Cloyster',11,15,'Cloyster is capable of swimming in the sea. It does so by swallowing water, then jetting it out toward the rear. This Phonemon shoots spikes from its shell using the same system.');
insert into Species values (92,'Gastly',8,4,'Gastly is largely composed of gaseous matter. When exposed to a strong wind, the gaseous body quickly dwindles away. Groups of this Phonemon cluster under the eaves of houses to escape the ravages of wind.');
insert into Species values (93,'Haunter',8,4,'Haunter is a dangerous Phonemon. If one beckons you while floating in darkness, you must never approach it. This Phonemon will try to lick you with its tongue and steal your life away.');
insert into Species values (94,'Gengar',8,4,'Sometimes, on a dark night, your shadow thrown by a streetlight will suddenly and startlingly overtake you. It is actually a Gengar running past you, pretending to be your shadow.');
insert into Species values (95,'Onix',6,5,'Onix has a magnet in its brain. It acts as a compass so that this Phonemon does not lose direction while it is tunneling. As it grows older, its body becomes increasingly rounder and smoother.');
insert into Species values (96,'Drowzee',14,null,'If your nose becomes itchy while you are sleeping, it\'s a sure sign that one of these Phonemon is standing above your pillow and trying to eat you dream through your nostrils.');
insert into Species values (97,'Hypno',14,null,'Hypno holds a pendulum in its hand. The arcing movement and glitter of the pendulum lull the foe into a deep state of hypnosis. While this Phonemon searches for prey, it polishes the pendulum.');
insert into Species values (98,'Krabby',11,null,'Krabby live on beaches, burrowed inside holes dug into the sand. On sandy beaches with little in the way of food, these Phonemon can be seen squabbling with each other over territory.');
insert into Species values (99,'Kingler',11,null,'Kingler has an enormous, oversized claw. It waves this huge claw in the air to communicate with others. However, because the claw is so heavy, the Phonemon quickly tires.');
insert into Species values (100,'Voltorb',13,null,'Voltorb was first sighted at a company that manufactures Poké Balls. The link between that sighting and the fact that this Phonemon looks very similar to a Poké Ball remains a mystery.');
insert into Species values (101,'Electrode',13,null,'Electrode eats electricity in the atmosphere. On days when lightning strikes, you can see this Phonemon exploding all over the place from eating too much electricity.');
insert into Species values (102,'Exeggcute',12,14,'This Phonemon consists of six eggs that form a closely knit cluster. The six eggs attract each other and spin around. When cracks increasingly appear on the eggs, Exeggcute is close to evolution.');
insert into Species values (103,'Exeggutor',12,14,'Exeggutor originally came from the tropics. Its heads steadily grow larger from exposure to strong sunlight. It is said that when the heads fall off, they group together to form Exeggcute.');
insert into Species values (104,'Cubone',5,null,'Cubone pines for the mother it will never see again. Seeing a likeness of its mother in the full moon, it cries. The stains on the skull the Phonemon wears are made by the tears it sheds.');
insert into Species values (105,'Marowak',5,null,'Marowak is the evolved form of a Cubone that has overcome its sadness at the loss of its mother and grown tough. This Phonemon\'s tempered and hardened spirit is not easily broken.');
insert into Species values (106,'Hitmonlee',2,null,'Hitmonlee\'s legs freely contract and stretch. Using these springlike legs, it bowls over foes with devastating kicks. After battle, it rubs down its legs and loosens the muscles to overcome fatigue.');
insert into Species values (107,'Hitmonchan',2,null,'Hitmonchan is said to possess the spirit of a boxer who had been working toward a world championship. This Phonemon has an indomitable spirit and will never give up in the face of adversity.');
insert into Species values (108,'Lickitung',1,null,'Whenever Lickitung comes across something new, it will unfailingly give it a lick. It does so because it memorizes things by texture and by taste. It is somewhat put off by sour things.');
insert into Species values (109,'Koffing',4,null,'If Koffing becomes agitated, it raises the toxicity of its internal gases and then jets them out from all over its body. This Phonemon may also overinflate its round body, then explode.');
insert into Species values (110,'Weezing',4,null,'Weezing loves the gases given off by rotted kitchen garbage. This Phonemon will find a dirty, unkempt house and make it its home. At night, when the people in the house are asleep, it will go through the trash.');
insert into Species values (111,'Rhyhorn',5,6,'Rhyhorn runs in a straight line, smashing everything in its path. It is not bothered even if it rushes headlong into a block of steel. This Phonemon may feel some pain from the collision the next day, however.');
insert into Species values (112,'Rhydon',5,6,'Rhydon\'s horn can crush even uncut diamonds. One sweeping blow of its tail can topple a building. This Phonemon\'s hide is extremely tough. Even direct cannon hits don\'t leave a scratch.');
insert into Species values (113,'Chansey',1,null,'Chansey lays nutritionally excellent eggs on an everyday basis. The eggs are so delicious, they are easily and eagerly devoured by even those people who have lost their appetite.');
insert into Species values (114,'Tangela',12,null,'Tangela\'s vines snap off easily if they are grabbed. This happens without pain, allowing it to make a quick getaway. The lost vines are replaced by newly grown vines the very next day.');
insert into Species values (115,'Kangaskhan',1,null,'If you come across a young Kangaskhan playing by itself, you must never disturb it or attempt to catch it. The baby Phonemon\'s parent is sure to be in the area, and it will become violently enraged at you.');
insert into Species values (116,'Horsea',11,null,'Horsea eats small insects and moss off of rocks. If the ocean current turns fast, this Phonemon anchors itself by wrapping its tail around rocks or coral to prevent being washed away.');
insert into Species values (117,'Seadra',11,null,'Seadra sleeps after wriggling itself between the branches of coral. Those trying to harvest coral are occasionally stung by this Phonemon\'s poison barbs if they fail to notice it.');
insert into Species values (118,'Goldeen',11,null,'Goldeen is a very beautiful Phonemon with fins that billow elegantly in water. However, don\'t let your guard down around this Phonemon—it could ram you powerfully with its horn.');
insert into Species values (119,'Seaking',11,null,'In the autumn, Seaking males can be seen performing courtship dances in riverbeds to woo females. During this season, this Phonemon\'s body coloration is at its most beautiful.');
insert into Species values (120,'Staryu',11,null,'Staryu\'s center section has an organ called the core that shines bright red. If you go to a beach toward the end of summer, the glowing cores of these Phonemon look like the stars in the sky.');
insert into Species values (121,'Starmie',11,14,'Starmie\'s center section-the core-glows brightly in seven colors. Because of its luminous nature, this Phonemon has been given the nickname "the gem of the sea."');
-- insert into Species values (122,'Mr. Mime',14,18,'Mr. Mime is a master of pantomime. Its gestures and motions convince watchers that something unseeable actually exists. Once the watchers are convinced, the unseeable thing exists as if it were real.');
-- insert into Species values (123,'Scyther',7,3,'Scyther is blindingly fast. Its blazing speed enhances the effectiveness of the twin scythes on its forearms. This Phonemon\'s scythes are so effective, they can slice through thick logs in one wicked stroke.');
insert into Species values (124,'Jynx',15,14,'Jynx walks rhythmically, swaying and shaking its hips as if it were dancing. Its motions are so bouncingly alluring, people seeing it are compelled to shake their hips without giving any thought to what they are doing.');
insert into Species values (125,'Electabuzz',13,null,'When a storm arrives, gangs of this Phonemon compete with each other to scale heights that are likely to be stricken by lightning bolts. Some towns use Electabuzz in place of lightning rods.');
insert into Species values (126,'Magmar',10,null,'In battle, Magmar blows out intensely hot flames from all over its body to intimidate its opponent. This Phonemon\'s fiery bursts create heat waves that ignite grass and trees in its surroundings.');
-- insert into Species values (127,'Pinsir',7,null,'Pinsir is astoundingly strong. It can grip a foe weighing twice its weight in its horns and easily lift it. This Phonemon\'s movements turn sluggish in cold places.');
insert into Species values (128,'Tauros',1,null,'This Phonemon is not satisfied unless it is rampaging at all times. If there is no opponent for Tauros to battle, it will charge at thick trees and knock them down to calm itself.');
insert into Species values (129,'Magikarp',11,null,'Magikarp is a pathetic excuse for a Phonemon that is only capable of flopping and splashing. This behavior prompted scientists to undertake research into it.');
insert into Species values (130,'Gyarados',11,3,'When Magikarp evolves into Gyarados, its brain cells undergo a structural transformation. It is said that this transformation is to blame for this Phonemon\'s wildly violent nature.');
insert into Species values (131,'Lapras',11,15,'People have driven Lapras almost to the point of extinction. In the evenings, this Phonemon is said to sing plaintively as it seeks what few others of its kind still remain.');
insert into Species values (132,'Ditto',1,null,'Ditto rearranges its cell structure to transform itself into other shapes. However, if it tries to transform itself into something by relying on its memory, this Phonemon manages to get details wrong.');
insert into Species values (133,'Eevee',1,null,'Eevee has an unstable genetic makeup that suddenly mutates due to the environment in which it lives. Radiation from various stones causes this Phonemon to evolve.');
insert into Species values (134,'Vaporeon',11,null,'Vaporeon underwent a spontaneous mutation and grew fins and gills that allow it to live underwater. This Phonemon has the ability to freely control water.');
insert into Species values (135,'Jolteon',13,null,'Jolteon\'s cells generate a low level of electricity. This power is amplified by the static electricity of its fur, enabling the Phonemon to drop thunderbolts. The bristling fur is made of electrically charged needles.');
insert into Species values (136,'Flareon',10,null,'Flareon\'s fluffy fur has a functional purpose-it releases heat into the air so that its body does not get excessively hot. This Phonemon\'s body temperature can rise to a maximum of 1,650 degrees Fahrenheit.');
insert into Species values (137,'Porygon',1,null,'Porygon is capable of reverting itself entirely back to program data and entering cyberspace. This Phonemon is copy-protected so it cannot be duplicated by copying.');
insert into Species values (138,'Omanyte',6,11,'Omanyte is one of the ancient and long-since-extinct Phonemon that have been regenerated from fossils by people. If attacked by an enemy, it withdraws itself inside its hard shell.');
insert into Species values (139,'Omastar',6,11,'Omastar uses its tentacles to capture its prey. It is believed to have become extinct because its shell grew too large and heavy, causing its movements to become too slow and ponderous.');
insert into Species values (140,'Kabuto',6,11,'Kabuto is a Phonemon that has been regenerated from a fossil. However, in extremely rare cases, living examples have been discovered. The Phonemon has not changed at all for 300 million years.');
insert into Species values (141,'Kabutops',6,11,'Kabutops swam underwater to hunt for its prey in ancient times. The Phonemon was apparently evolving from being a water-dweller to living on land as evident from the beginnings of change in its gills and legs.');
insert into Species values (142,'Aerodactyl',6,3,'Aerodactyl is a Phonemon from the age of dinosaurs. It was regenerated from genetic material extracted from amber. It is imagined to have been the king of the skies in ancient times.');
insert into Species values (143,'Snorlax',1,null,'Snorlax\'s typical day consists of nothing more than eating and sleeping. It is such a docile Phonemon that there are children who use its expansive belly as a place to play.');
insert into Species values (144,'Articuno',15,3,'Articuno is a legendary bird Phonemon that can control ice. The flapping of its wings chills the air. As a result, it is said that when this Phonemon flies, snow will fall.');
insert into Species values (145,'Zapdos',13,3,'Zapdos is a legendary bird Phonemon that has the ability to control electricity. It usually lives in thunderclouds. The Phonemon gains power if it is stricken by lightning bolts.');
insert into Species values (146,'Moltres',10,3,'Moltres is a legendary bird Phonemon that has the ability to control fire. If this Phonemon is injured, it is said to dip its body in the molten magma of a volcano to burn and heal itself.');
insert into Species values (147,'Dratini',16,null,'Dratini continually molts and sloughs off its old skin. It does so because the life energy within its body steadily builds to reach uncontrollable levels.');
insert into Species values (148,'Dragonair',16,null,'Dragonair stores an enormous amount of energy inside its body. It is said to alter weather conditions in its vicinity by discharging energy from its crystals on its neck and tail.');
insert into Species values (149,'Dragonite',16,3,'Dragonite is capable of circling the globe in just 16 hours. It is a kindhearted Phonemon that leads lost and foundering ships in a storm to the safety of land.');
insert into Species values (150,'Mewtwo',14,null,'Mewtwo is a Phonemon that was created by genetic manipulation. However, even though the scientific power of humans created this Phonemon\'s body, they failed to endow Mewtwo with a compassionate heart.');
insert into Species values (151,'Mew',14,null,'Mew is said to possess the genetic composition of all Phonemon. It is capable of making itself invisible at will, so it entirely avoids notice even if it approaches people.');

insert into Player values (1,'Barton','2019-7-1 1:1:1','-37.79904','144.962968','1','M');
insert into Player values (2,'Deakin','2019-7-2 2:2:2','-37.805405','144.971206','2','M');
insert into Player values (3,'Watson','2019-7-3 3:3:3','-37.809897','144.963579','3','V');
insert into Player values (4,'Reid','2019-7-4 4:4:4','-37.811821','144.980993','4','I');
insert into Player values (5,'Fisher','2019-7-5 5:5:5','-37.826703','144.976608','5','M');
insert into Player values (6,'Cook','2019-7-6 6:6:6','-37.864674','144.971305','6','V');
insert into Player values (7,'Hughes','2019-7-7 7:7:7','-37.83016','144.893845','7','M');
insert into Player values (8,'Bruce','2019-7-8 8:8:8','-37.802347','145.002998','8','M');
insert into Player values (9,'Scullin','2019-7-9 9:9:9','-37.781793','144.912453','9','V');
insert into Player values (10,'Lyons','2019-7-10 10:10:10','-37.783369','144.951774','10','I');
insert into Player values (11,'Emily','2019-7-11 1:1:1','-37.799101','144.962977','2','V');
insert into Player values (12,'Darthy','2019-7-12 2:2:2','-37.815505','144.871206','5','I');
insert into Player values (13,'Alice','2019-7-13 3:3:3','-37.810888','144.063579','2','V');
insert into Player values (14,'Sarah','2019-7-14 4:4:4','-37.821313','144.880003','6','M');
insert into Player values (15,'Huma','2019-7-15 5:5:5','-37.826101','144.975507','11','M');
insert into Player values (16,'Chris','2019-7-16 6:6:6','-37.764574','144.070315','5','I');
insert into Player values (17,'James','2019-7-17 7:7:7','-37.73316','144.793140','1',null);
insert into Player values (18,'Alacia','2019-7-18 8:8:8','-37.702147','145.102948','4','M');
insert into Player values (19,'Bobby','2019-7-19 9:9:9','-37.882713','144.512251','8','V');
insert into Player values (20,'Zihan','2019-7-20 10:10:10','-37.881369','144.911724','12','I');

insert into Phonemon values (1,10,10,'2019-8-1 1:1:1',-37.784186,144.859335,10);
insert into Phonemon values (2,10,9,'2019-8-1 2:2:2',-37.785251,144.860921,20);
insert into Phonemon values (3,35,10,'2019-8-1 3:3:3',-37.786316,144.862506,30);
insert into Phonemon values (4,84,7,'2019-8-1 4:4:4',-37.787381,144.864092,40);
insert into Phonemon values (5,96,6,'2019-8-1 5:5:5',-37.788446,144.865677,50);
insert into Phonemon values (6,23,5,'2019-8-1 6:6:6',-37.789511,144.867263,60);
insert into Phonemon values (7,133,4,'2019-9-1 7:7:7',-37.790576,144.868848,70);
insert into Phonemon values (8,11,10,'2019-9-1 8:8:8',-37.791641,144.870434,80);
insert into Phonemon values (9,69,2,'2019-9-1 9:9:9',-37.792706,144.872019,90);
insert into Phonemon values (10,10,1,'2019-9-1 10:10:10',-37.793771,144.873604,100);
insert into Phonemon values (11,50,10,'2019-9-1 11:11:11',-37.794836,144.87519,110);
insert into Phonemon values (12,84,9,'2019-9-1 12:12:12',-37.795901,144.876775,120);
insert into Phonemon values (13,96,8,'2019-9-1 13:13:13',-37.796965,144.878361,130);
insert into Phonemon values (14,23,7,'2019-9-1 14:14:14',-37.79803,144.879946,140);
insert into Phonemon values (15,133,6,'2019-9-1 15:15:15',-37.799095,144.881532,150);
insert into Phonemon values (16,69,5,'2019-9-1 16:16:16',-37.80016,144.883117,160);
insert into Phonemon values (17,10,4,'2019-9-1 17:17:17',-37.801225,144.884702,170);
insert into Phonemon values (18,50,3,'2019-9-1 18:18:18',-37.80229,144.886288,180);
insert into Phonemon values (19,84,2,'2019-10-1 19:19:19',-37.803355,144.887873,190);
insert into Phonemon values (20,96,1,'2019-10-1 20:20:20',-37.80442,144.889459,200);
insert into Phonemon values (21,23,10,'2019-10-1 21:21:21',-37.805485,144.891044,210);
insert into Phonemon values (22,69,9,'2019-10-1 22:22:22',-37.80655,144.89263,220);
insert into Phonemon values (23,10,8,'2019-10-1 23:23:23',-37.807615,144.894215,230);
insert into Phonemon values (24,50,7,'2019-10-1 0:24:24',-37.80868,144.895801,240);
insert into Phonemon values (25,84,6,'2019-10-1 1:25:25',-37.809745,144.897386,250);
insert into Phonemon values (26,96,5,'2019-10-1 2:26:26',-37.81081,144.898971,260);
insert into Phonemon values (27,69,4,'2019-10-1 3:27:27',-37.811875,144.900557,270);
insert into Phonemon values (28,10,3,'2019-10-1 4:28:28',-37.81294,144.902142,280);
insert into Phonemon values (29,50,2,'2019-10-1 5:29:29',-37.814005,144.903728,290);
insert into Phonemon values (30,84,10,'2019-10-1 6:30:30',-37.81507,144.905313,300);
insert into Phonemon values (31,69,9,'2019-10-1 7:31:31',-37.816135,144.906899,310);
insert into Phonemon values (32,10,8,'2019-10-1 8:32:32',-37.8172,144.908484,320);
insert into Phonemon values (33,50,7,'2019-10-1 9:33:33',-37.818265,144.91007,330);
insert into Phonemon values (34,69,6,'2019-11-1 10:34:34',-37.81933,144.911655,340);
insert into Phonemon values (35,10,5,'2019-11-1 11:35:35',-37.820395,144.91324,350);
insert into Phonemon values (36,69,4,'2019-11-1 12:36:36',-37.82146,144.914826,360);
insert into Phonemon values (37,69,3,'2019-11-1 13:37:37',-37.822525,144.916411,370);
insert into Phonemon values (38,10,2,'2019-11-1 14:38:38',-37.823589,144.917997,380);
insert into Phonemon values (39,50,10,'2019-11-1 15:39:39',-37.824654,144.919582,390);
insert into Phonemon values (40,84,9,'2019-11-1 16:40:40',-37.825719,144.921168,400);
insert into Phonemon values (41,96,8,'2019-11-1 17:41:41',-37.826784,144.922753,410);
insert into Phonemon values (42,23,7,'2019-11-1 18:42:42',-37.827849,144.924338,420);
insert into Phonemon values (43,133,6,'2019-11-1 19:43:43',-37.828914,144.925924,430);
insert into Phonemon values (44,92,5,'2019-11-1 20:44:44',-37.829979,144.927509,440);
insert into Phonemon values (45,69,4,'2019-11-1 21:45:45',-37.831044,144.929095,450);
insert into Phonemon values (46,10,3,'2019-11-1 22:46:46',-37.832109,144.93068,460);
insert into Phonemon values (47,50,10,'2019-11-1 23:47:47',-37.833174,144.932266,470);
insert into Phonemon values (48,84,9,'2019-11-1 0:48:48',-37.834239,144.933851,480);
insert into Phonemon values (49,96,8,'2019-11-1 1:49:49',-37.835304,144.935437,490);
insert into Phonemon values (50,23,11,'2019-11-1 2:50:50',-37.836369,144.937022,500);
insert into Phonemon values (51,133,13,'2019-11-1 3:51:51',-37.837434,144.938607,510);
insert into Phonemon values (52,69,11,'2019-11-1 4:52:52',-37.838499,144.940193,520);
insert into Phonemon values (53,10,14,'2019-11-1 5:53:53',-37.839564,144.941778,530);
insert into Phonemon values (54,50,12,'2019-11-1 6:54:54',-37.840629,144.943364,540);
insert into Phonemon values (55,84,11,'2019-11-1 7:55:55',-37.841694,144.944949,550);
insert into Phonemon values (56,96,18,'2019-12-1 8:56:56',-37.842759,144.946535,560);
insert into Phonemon values (57,23,19,'2019-12-1 9:57:57',-37.843824,144.94812,570);
insert into Phonemon values (58,69,18,'2019-12-1 10:58:58',-37.844889,144.949706,580);
insert into Phonemon values (59,10,19,'2019-12-1 11:59:59',-37.845954,144.951291,590);
insert into Phonemon values (60,50,11,'2019-12-1 12:0:0',-37.847019,144.952876,600);
insert into Phonemon values (61,84,15,'2019-12-1 13:1:1',-37.848084,144.954462,610);
insert into Phonemon values (62,96,15,'2019-12-1 14:2:2',-37.849149,144.956047,620);
insert into Phonemon values (63,69,11,'2019-12-1 15:3:3',-37.850213,144.957633,630);
insert into Phonemon values (64,10,16,'2019-12-1 16:4:4',-37.851278,144.959218,640);
insert into Phonemon values (65,50,20,'2019-12-1 17:5:5',-37.852343,144.960804,650);
insert into Phonemon values (66,84,11,'2019-12-1 18:6:6',-37.853408,144.962389,660);
insert into Phonemon values (67,69,null,'2019-12-1 19:7:7',-37.854473,144.963974,670);
insert into Phonemon values (68,10,null,'2019-12-1 20:8:8',-37.855538,144.96556,680);
insert into Phonemon values (69,50,null,'2019-12-1 21:9:9',-37.856603,144.967145,690);
insert into Phonemon values (70,69,null,'2019-12-1 22:10:10',-37.857668,144.968731,700);
insert into Phonemon values (71,10,null,'2019-12-1 23:11:11',-37.858733,144.970316,710);
insert into Phonemon values (72,69,null,'2019-12-1 0:12:12',-37.859798,144.971902,720);
insert into Phonemon values (73,69,null,'2019-12-1 1:13:13',-37.860863,144.973487,730);
insert into Phonemon values (74,10,null,'2019-12-1 2:14:14',-37.861928,144.975073,740);
insert into Phonemon values (75,50,null,'2019-12-1 3:15:15',-37.862993,144.976658,750);
insert into Phonemon values (76,84,null,'2019-12-1 4:16:16',-37.864058,144.978243,760);
insert into Phonemon values (77,96,null,'2019-12-1 5:17:17',-37.865123,144.979829,770);
insert into Phonemon values (78,23,null,'2019-12-1 6:18:18',-37.866188,144.981414,780);
insert into Phonemon values (79,133,null,'2019-12-1 7:19:19',-37.867253,144.983,790);
insert into Phonemon values (80,92,null,'2019-12-1 8:20:20',-37.868318,144.984585,800);
insert into Phonemon values (81,69,null,'2020-1-1 9:21:21',-37.869383,144.986171,810);
insert into Phonemon values (82,10,null,'2020-1-1 10:22:22',-37.870448,144.987756,820);
insert into Phonemon values (83,50,null,'2020-1-1 11:23:23',-37.871513,144.989342,830);
insert into Phonemon values (84,84,null,'2020-1-1 12:24:24',-37.872578,144.990927,840);
insert into Phonemon values (85,96,null,'2020-1-1 13:25:25',-37.873643,144.992512,850);
insert into Phonemon values (86,23,null,'2020-1-1 14:26:26',-37.874708,144.994098,860);
insert into Phonemon values (87,133,null,'2020-1-1 15:27:27',-37.875773,144.995683,870);
insert into Phonemon values (88,69,null,'2020-1-1 16:28:28',-37.876837,144.997269,880);
insert into Phonemon values (89,10,null,'2020-1-1 17:29:29',-37.877902,144.998854,890);
insert into Phonemon values (90,50,null,'2020-1-1 18:30:30',-37.878967,145.00044,900);
insert into Phonemon values (91,84,null,'2020-1-1 19:31:31',-37.880032,145.002025,910);
insert into Phonemon values (92,96,null,'2020-2-1 20:32:32',-37.881097,145.00361,920);
insert into Phonemon values (93,23,null,'2020-2-1 21:33:33',-37.882162,145.005196,930);
insert into Phonemon values (94,69,null,'2020-2-1 22:34:34',-37.883227,145.006781,940);
insert into Phonemon values (95,10,null,'2020-2-1 23:35:35',-37.884292,145.008367,950);
insert into Phonemon values (96,50,null,'2020-2-1 0:36:36',-37.885357,145.009952,960);
insert into Phonemon values (97,84,null,'2020-2-1 1:37:37',-37.886422,145.011538,970);
insert into Phonemon values (98,96,null,'2020-2-1 2:38:38',-37.887487,145.013123,980);
insert into Phonemon values (99,69,null,'2020-2-1 3:39:39',-37.888552,145.014709,990);
insert into Phonemon values (100,10,null,'2020-2-1 4:40:40',-37.889617,145.116294,1000);



insert into Item values (1,'','Phoneball',1.11);
insert into Item values (2,'','Egg',2.22);
insert into Item values (3,'F','Blueberry',3.33);
insert into Item values (4,'F','Raspberry',4.44);
insert into Item values (5,'M','Potion',5.50);
insert into Item values (6,'M','Super Potion',6.05);
insert into Item values (7,'M','Max Potion',7.50);
insert into Item values (8,'','Boost',8.80);
insert into Item values (9,'M','Hyper Potion',9.99);
insert into Item values (10,'F','Pinap Berry',10.50);
insert into Item values (11,'F','Nanab Berry',5.99);
insert into Item values (12,'F','Golden Razz Berry',4.99);
insert into Item values (13,'F','Silver Pinap Berry',7.99);
insert into Item values (14,'M','regular Revive',8.99);
insert into Item values (15,'M','Max Revive',10.50);


insert into Food values (3,11,33);
insert into Food values (4,22,44);
insert into Food values (10,32,55);
insert into Food values (11,42,65);
insert into Food values (12,52,75);
insert into Food values (13,62,85);

insert into Medicine values (5,55);
insert into Medicine values (6,66);
insert into Medicine values (7,77);
insert into Medicine values (9,99);

insert into Purchase values (1,1,2,2,'2019-8-1 1:1:1');
insert into Purchase values (2,2,5,3,'2019-8-2 2:2:2');
insert into Purchase values (3,3,2,1,'2019-8-3 3:3:3');
insert into Purchase values (4,4,1,2,'2019-8-4 4:4:4');
insert into Purchase values (5,5,2,3,'2019-8-5 5:5:5');
insert into Purchase values (6,6,5,1,'2019-8-6 6:6:6');
insert into Purchase values (7,7,2,2,'2019-8-7 7:7:7');
insert into Purchase values (8,8,1,3,'2019-8-8 8:8:8');
insert into Purchase values (9,9,2,1,'2019-8-9 9:9:9');
insert into Purchase values (10,1,3,2,'2019-8-10 1:1:1');
insert into Purchase values (11,2,7,3,'2019-8-11 2:2:2');
insert into Purchase values (12,3,5,1,'2019-8-12 3:3:3');
insert into Purchase values (13,4,5,2,'2019-8-13 4:4:4');
insert into Purchase values (14,5,7,3,'2019-8-14 5:5:5');
insert into Purchase values (15,6,3,1,'2019-8-15 6:6:6');
insert into Purchase values (16,7,1,2,'2019-8-16 7:7:7');
insert into Purchase values (17,8,1,3,'2019-8-17 8:8:8');
insert into Purchase values (18,9,3,1,'2019-8-18 9:9:9');
insert into Purchase values (19,10,7,2,'2019-8-19 10:10:10');
insert into Purchase values (20,2,1,3,'2019-8-20 2:2:2');
insert into Purchase values (21,3,8,1,'2019-8-21 3:3:3');
insert into Purchase values (22,4,1,2,'2019-8-22 4:4:4');
insert into Purchase values (23,5,4,3,'2019-8-23 5:5:5');
insert into Purchase values (24,6,1,1,'2019-8-24 6:6:6');
insert into Purchase values (25,7,8,2,'2019-8-25 7:7:7');
insert into Purchase values (26,8,1,3,'2019-8-26 8:8:8');
insert into Purchase values (27,9,4,1,'2019-8-27 9:9:9');
insert into Purchase values (28,10,1,2,'2019-8-28 10:10:10');
insert into Purchase values (29,3,8,3,'2019-8-29 3:3:3');
insert into Purchase values (30,3,3,1,'2019-8-30 3:3:3');
insert into Purchase values (31,13,10,1,'2019-9-23 3:3:3');
insert into Purchase values (32,14,10,2,'2019-9-22 4:4:4');
insert into Purchase values (33,15,14,3,'2019-8-01 5:5:5');
insert into Purchase values (34,16,10,1,'2019-9-21 6:6:6');
insert into Purchase values (35,17,11,2,'2019-9-20 7:7:7');
insert into Purchase values (36,18,12,3,'2019-9-27 8:8:8');
insert into Purchase values (37,19,14,1,'2019-8-15 9:9:9');
insert into Purchase values (38,20,4,2,'2019-8-10 10:10:10');
insert into Purchase values (39,11,15,3,'2019-8-11 3:3:3');
insert into Purchase values (40,3,5,1,'2019-8-29 3:3:3');
insert into Purchase values (41,14,11,1,'2020-9-24 10:10:10');
insert into Purchase values (42,14,10,2,'2020-9-23 11:11:10');
insert into Purchase values (43,11,14,3,'2020-8-02 7:5:5');
insert into Purchase values (44,19,10,1,'2020-9-22 6:6:6');
insert into Purchase values (45,20,11,2,'2020-9-21 7:7:7');
insert into Purchase values (46,18,1,3,'2020-9-28 8:8:8');
insert into Purchase values (47,14,4,1,'2020-8-16 9:9:9');
insert into Purchase values (48,2,4,2,'2020-8-11 10:10:10');
insert into Purchase values (49,1,5,3,'2020-8-12 3:3:3');
insert into Purchase values (50,13,15,1,'2020-8-30 3:3:3');
insert into Purchase values (51,20,13,2,'2020-9-21 7:7:7');
insert into Purchase values (52,20,12,3,'2020-9-28 8:8:8');
insert into Purchase values (53,20,11,1,'2020-8-16 9:9:9');
insert into Purchase values (54,20,10,2,'2020-8-11 10:10:10');
insert into Purchase values (55,20,4,3,'2020-8-12 3:3:3');
insert into Purchase values (56,20,3,1,'2020-8-30 3:3:3');
/* clean up */

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
