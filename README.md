# Watan


# 1. Overview
In this section, we will discuss, on a high level, how our game of “Students of Watan” is implemented and its structure. For more detailed implementation overview, feel free to refer to our UML below to learn more about how our program functions as a whole. Our program can be broken down into six major parts: 
Controller, which handles input and output alongside main, 
Board, which contains all pieces required to construct a game, 
Player that interacts with the board and controller to change the state of the game,
TextDisplay that communicates with Controller to display the board,
Tile that holds the information for its value (dice roll) and resource, and
Criteria and Goals, which are achievable objects by the players.
These six sections all interact with each other that makes the game playable. The subsection below will go into detail about the structure of each part and how they interact.

## 1.1 Controller
Our decision to use Member-Viewer-Controller (MVC) design pattern meant that it was necessary for us to implement Controller. Controller’s main job is to handle the majority of the input from the user and calls methods from other parts of the program to obtain the valid output for the program. Controller collaborates with main to set up the game, continuously update TextDisplay when needed, and catches all exceptions that other classes throw. Controller is also responsible for distributing the random engine that we use, as well as handling all possible commands that user can input. The commands are categorized as beginning-of-turn commands and end-of-turn commands, where it restricts the input for some commands that are not allowed at certain stages of the game. The available list of commands you can input for end-of-turn commands can be viewed by typing “help”.

## 1.2 Board
Board contains ALL of the necessary information that the game needs. In a way, this is our wrapper class that owns all of the models (employing MVC design pattern). Board is responsible for constructing all of the necessary classes we need to play the game, and has two separate constructor for the case where we load from save. It keeps track of the location for the goose, and also is responsible for saving the current game state. Controller is the main class that it directly interacts with.
1.3 Player
Player is an Observer in our program. Player “owns” criteria and goals, but formally in terms of HAS_A. Each player just keeps track of the index (or id) of the criteria and goals it owns and passes the id up to the board which allows any modifications to be made. Each player is also keeping track of resources, which is a ResourceVector class we created (a variant of an int array of length 5 that has methods we would use throughout our program). This gets updated whenever goals and criteria call notify on the player it should have. Player HAS_A dice, which is default initialized as FairDice (random dice roll), that can be modified into a LoadedDice (cheating!).

## 1.4 TextDisplay
Coming back to our MVC pattern, we have TextDisplay as the View. The fields it has are mostly string arrays which keeps track of the current state of the board. When the board updates, TextDisplay is listening for changes similar to Player class. This makes our TextDisplay an Observer as well. Notify is used to update the correct location with the new information, and the initialization method is very similar to how Board is set up.

## 1.5 Dice
Our dice class is implemented by defining a base Dice class with further subclasses for LoadedDice and FairDice. We were not able to come up with a solution that lets Player abstract itself away from this design: while Player can roll the dice without knowing what kind they are, they do have to explicitly construct the dice using the subclass constructor when the PLayer::load or Player::fair methods are called.

## 1.6 Tile
Tile is one of our main subjects in the game. A Tile has fields including an id, its resource type, value (dice roll) and a vector of criteria, which are the tile’s observers. Whenever the dice is rolled, Tiles have the method valueEqual(int val) that checks if the tile’s value is equal to the roll, and calls notifyObservers() to notify the criteria it has.

## 1.7 Criteria and Goals
The Classes Goal and Criteria are both subjects. Their main job is to notify the TextDisplay on any changes (such as whenever a player tries to build or improve). They keep track of their owner with a player raw pointer (HAS_A) with the corresponding id. In order to check the valid placement for each criterion and goal, we had to implement a vector that keeps track of its neighbouring criteria for goals and neighbouring goals for criteria. The main difference between Criteria and Goals is that Criteria are also observers. Tiles notify the criteria, and each criterion receives that update and then calls notify on its observers to make changes to the players that it owns, for resource distribution purposes. 



# 3. Design
The major design pattern we used in terms of the structure of our game is MVC pattern. As we mentioned above, Controller and main takes input and handles output, View is the TextDisplay and Models are every class that are included in board. The interaction between the classes are handled with the Observer design pattern, where subjects are Tiles, Goals and Criteria. Tiles notify Criteria to distribute resources when the matching dice value is rolled, which are its only observers. We decided not to add TextDisplay as its observers since the Geese is handled by the Controller and the Board (the only thing that changes in Tile are geese which requires update for TextDisplay). Goals and Criteria notify the TextDisplay when it gets built, in order to update the View, and Criteria additionally notifies Player in the case of resource distribution, mentioned above. 
On top of the Observer Pattern, we used Visitor design pattern for the models we have. This is because they each depend on what type of the observer/subject they are, which means it requires double dispatch. Subject abstract class has a pure virtual notifyObservers() method, in which each subject overrides in order to let the observers know which subject is notifying them. Similarly, Observer abstract class has different versions of pure virtual notify method, that takes in different subject as its parameter. This means that there is a double dispatch for the subject that are notifying and the observers that are being notified to exhibit different behaviour on each of the type it takes in.
Our program also uses a template method pattern in Criteria, since Criteria have four different versions: NoCriteria, Assignment, Midterm and Exam, where Criteria provides the common methods that all the criteria need, such as goalIsOwnedBy(Player *) (checks if the placement is valid) or nbrCriteriaExist(), and some criteria-type specific method for each type. 
For a player’s dice method, we were originally trying to implement them using the pImple idiom. However, once we tried to implement it, there were challenges we faced and complications which made the program unnecessarily inefficient. Therefore, we scrapped the bridge design pattern and just created it using template method pattern, where dice provides some basic methods and two dice subclasses, FairDice and LoadedDice.



# 4. Resilience to Change
We designed our project to be relatively resilient to change, but for convenience’s (and time’s) sake did not implement many of the design items concretely.  We have outlined the main ways our program can accommodate change in depth below.

## 4.1 MVC design
We used the MVC design pattern in this project to abstract the user interface away from the implementation of the game. All input and output is handled by the controller (Controller and main, in tandem), the view is handled by TextDisplay, and the model is completely contained within Board. We can then change at will the design and implementation of the model and view without impacting the user interaction whatsoever. Thus, our user interface is resilient to design changes. 

## 4.2 View (TextDisplay)
Our view is completely contained in the TextDisplay class, which is an observer of most game assets. Anything that will change with time and must be displayed as part of the board, namely the Goal and Criteria classes, have the view as an observer who will be notified when their state changes. If we chose to add more game assets that change with time, like making Tile allocate different resources dynamically and thus needing to change its label, we could easily add a TextDisplay::notify(Tile &) method to update TextDisplay’s internal representation of Tiles. Should we decide to add different sized boards, we could easily refactor TextDisplay into a TextBase class, defining the interface, and move our specific implementation of TextDisplay into a subclass. We chose not to do this in the current version of the project since it would add unneeded complexity to our design, without any gain in flexibility for our purposes. However, the change is trivial.

## 4.2 Model (Board)
Our board class contains the model entirely. This is useful since it abstracts away the internals of the model from the controller. The controller does not know that we have a Player, nor a Goal, nor any of the other game assets, except through the interface of Board. To accommodate changes to the board shape and size or the assets it contains, we could have defined a BoardBase class, and made our specific version of Board a subclass of it, which would have made adding a new board shape only a matter of adding a new subclass of the base class. We didn’t do this, though, because it would have added to the complexity of our design for no gain in performance. It would be nearly trivial to implement this change, though, needing only to move the Board-specific methods (the constructors) to a subclass and retain the other methods. 

### 4.2.1 Tiles
As described in more depth in section 5.7 below, our Tile class is extremely resilient to changes to the kinds and amounts of resources it can provide. It would be as simple as adding a method to board that would update Tile’s internal ResourceArray.
### 4.2.2 Criteria
Our criteria class at the moment is somewhat fragile. In a rush to ensure our program was correct, we added many default parameters and various edge case checks. We would need to significantly change the code to make our implementation resilient to change. However, we still managed to create a design that is resilient: and further upgrades past an exam (for example a thesis) would entail only adding a new class and changing the subclass directly before it to add a constructor for the new class. Each subclass is responsible for making sure its placement is valid: if we implemented a new subclass Thesis, it would simply need to implement its own Thesis::newUpdatedCriteria() and Thesis::isValidPlacement() methods. The Template Method pattern  in Criteria::upgradeCriteria takes care of the rest.