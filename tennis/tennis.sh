#!/bin/bash
#David Gurovich 318572047
player1_current_points=50
player2_current_points=50
player1_pick_points=0
player2_pick_points=0
game_on=true
part_of_field=0
responce=""
winner=0
turn=0


print_game_board(){
    middle_row=""
case "$part_of_field" in 
"0")
    middle_row=" |       |       O       |       | "
    ;;

"1")
    middle_row=" |       |       #   O   |       | "
    ;;

"2")
    middle_row=" |       |       #       |   O   | "
    ;;

"3")
    middle_row=" |       |       #       |       |O"
    ;;

"-1")
    middle_row=" |       |   O   #       |       | "
    ;;

"-2")
    middle_row=" |   O   |       #       |       | "
    ;;

"-3")
    middle_row="O|       |       #       |       | "
    ;;

    esac
    
    echo " Player 1: ${player1_current_points}         Player 2: ${player2_current_points} "
    echo " --------------------------------- "
    echo " |       |       #       |       | "
    echo " |       |       #       |       | "
    echo "$middle_row"
    echo " |       |       #       |       | "
    echo " |       |       #       |       | "
    echo " --------------------------------- "
    if  [ $part_of_field == 0 ]  ;
    then
        if [ $turn != 0 ] ;
        then
            echo -e "       Player 1 played: ${player1_pick_points}\n       Player 2 played: ${player2_pick_points}\n\n"
        else
            echo ""
            echo ""
        fi
    elif [ $part_of_field == -3 ] ;
        then
            echo ""
            echo ""
            echo -e "       Player 1 played: ${player1_pick_points}\n       Player 2 played: ${player2_pick_points}\n\n"
    elif [ $part_of_field == 3 ] ;
        then
            echo ""
            echo ""    
            echo -e "       Player 1 played: ${player1_pick_points}\n       Player 2 played: ${player2_pick_points}\n\n"
    else
        echo -e "       Player 1 played: ${player1_pick_points}\n       Player 2 played: ${player2_pick_points}\n\n"
    fi
}

regex='^[0-9]+$'
player1_pick(){
    echo "PLAYER 1 PICK A NUMBER: "
    read -s player1_pick_points
  if ! [[  $player1_pick_points =~ $regex ]] ;
  then 
    echo "NOT A VALID MOVE !"
    return 1 ;
  elif   (($player1_pick_points > $player1_current_points))   ;
  then 
    echo "NOT A VALID MOVE !"
    return 1 ;
  else
    ((turn=turn+1))
    return 0 ;
  fi 
}

player2_pick(){
    echo  "PLAYER 2 PICK A NUMBER: "
    read -s player2_pick_points
  if ! [[  $player2_pick_points =~ $regex ]] ;
  then 
    echo "NOT A VALID MOVE !"
    return 1 ;
  elif   (($player2_pick_points > $player2_current_points))   ;
  then 
    echo "NOT A VALID MOVE !"
    return 1 ;
  else
    return 0 ;
  fi 
}

reduce_points (){
    player1_current_points=$(expr $player1_current_points - $player1_pick_points)
    player2_current_points=$(expr $player2_current_points - $player2_pick_points)
}

players_pick_numbers(){
    player1_pick
    while [ $? == 1 ]
    do
        player1_pick
    done
    player2_pick
    while [ $? == 1 ]
    do
        player1_pick
        while [ $? == 1 ]
        do
            player1_pick
        done
        player2_pick
    done
    if [ $player1_pick_points \> $player2_pick_points ]
    then
        if [ $part_of_field == -1 ]
        then
            ((part_of_field=part_of_field+2))
            reduce_points
        elif [ $part_of_field == 2 ]
        then
            ((part_of_field=part_of_field+1))
            reduce_points
            winner=1
            game_on=false
        else
            reduce_points
            ((part_of_field=part_of_field+1))
        fi
    elif [ $player2_pick_points \> $player1_pick_points ]
     then
        if [ $part_of_field == 1 ]
        then
            reduce_points
            ((part_of_field=part_of_field-2))
        elif [ $part_of_field == -2 ]
        then
            reduce_points
            ((part_of_field=part_of_field-1))
            winner=2
            game_on=false
            echo "$game_on"
        else
            reduce_points
            ((part_of_field=part_of_field-1))
        fi
    else
        reduce_points
    fi
}

check_winner(){
    if [ $winner == 1 ] ;
    then
        print_game_board
        echo "PLAYER 1 WINS !"
        game_on=false
    elif [ $winner == 2 ] ;
    then
        print_game_board
        echo "PLAYER 2 WINS !"
        game_on=false
    else                                              # ball is not in foul zone
        if [ $player1_current_points == 0 ]  ;
        then
            if [ $player2_current_points == 0 ] ;
            then
                if [ $part_of_field == 0 ] ;
                then
                    game_on=false
                    print_game_board
                    echo "IT'S A DRAW !"
                elif [ $part_of_field > 0  ]
                then
                    game_on=false
                    winner=2
                    print_game_board
                    echo "PLAYER 2 WINS !"
                else
                    game_on=false
                    winner=1
                    print_game_board
                    echo "PLAYER 1 WINS !"
                fi 
            fi
        elif [ $player2_current_points == 0 ]
        then
            game_on=false
            winner=1
            print_game_board
            echo "PLAYER 1 WINS !"
        else
           ((turn=turn+1))
        fi
    fi
}




while $game_on
do
print_game_board
players_pick_numbers
check_winner
if  [ ! $game_on ]
then
    exit
fi
done
exit



