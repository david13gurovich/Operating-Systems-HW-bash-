#!bin/bash/
#David Gurovich 318572047
player1_current_points=50
player2_current_points=50
player1_pick_points=0
player2_pick_points=0
game_on=true
part_of_field=0
responce=""
winner=0
print_game_board(){
    middle_row=""
    case $part_of_field in 
    0)
    $middle_row=" |       |       O       |       | "
    ;;

    1)
    $middle_row=" |       |       #   O   |       | "
    ;;

    2)
    $middle_row=" |       |       #       |   O   | "
    ;;

    3)
    $middle_row=" |       |       #       |       |O"
    ;;

    -1)
    $middle_row=" |       |   O   #       |       | "
    ;;

    -2)
    $middle_row=" |   O   |       #       |       | "
    ;;

    -3)
    $middle_row="O|       |       #       |       | "
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
    if [[ $part_of_field != 0 ] && [ $part_of_field != 3 ] && [ $part_of_field != 3 ]]
    then
        echo -e "       Player 1 played: ${player1_pick_points}\n       Player 2 played: ${player2_pick_points}\n\n"
    else
        echo ""
        echo ""
}

player1_pick(){
    echo -n "PLAYER 1 PICK A SQUARE: "
    read -s player1_pick_points
  if [[  $player1_pick_points =~ ^[0-9]+$ ]] || [[  $player1_current_points \< $player1_pick_points  ]]
  then 
    echo "NOT A VALID MOVE !"
    return = false
  else
    return = true
  fi 
}
player2_pick(){
    echo -n "PLAYER 2 PICK A SQUARE: "
    read -s player2_pick_points
  if [[  $player2_pick_points =~ ^[0-9]+$ ]] || [[  $player2_current_points \< $player2_pick_points  ]]
  then 
    echo "NOT A VALID MOVE !"
    return = false
  else
    return = true
  fi 
}
reduce_points (){
    let $player1_current_points=$player1_current_points-$player1_pick_points
    let $player2_current_points=$player2_current_points-$player2_pick_points
}
players_pick_numbers(){
    player1_pick
    while [ $? == false ]
    do
        player1_pick
    done
    player2_pick
    while [ $? == false ]
    do
        player1_pick
        while [ $? == false ]
        do
            player1_pick
        done
        player2_pick
    done
    if [ $player1_pick_points \> $player2_pick_points ]
    then
        if [ $part_of_field == 1 ]
        then
            $part_of_field=-1
            reduce_points
        elif [ $part_of_field == -2]
        then
            $part_of_field == -3
            reduce_points
            $winner=1
            $game_on= false
        else
            reduce_points
            ((part_of_field=part_of_field-1))
        fi
    elif [ $player2_pick_points \> $player1_pick_points ]
     then
        if [ $part_of_field == -1 ]
        then
            reduce_points
            $part_of_field=1
        elif [ $part_of_field == 2]
        then
            reduce_points
            $part_of_field == 3
            $winner=2
            $game_on= false
        else
            reduce_points
            ((part_of_field=part_of_field+1))
        fi
    else
        reduce_points
    fi
}
check_winner(){
    if [ $winner == 1]
    then
        echo "PLAYER 1 WINS !"
        $game_on= false
    elif [ $winner == 2 ]
    then
        echo "PLAYER 2 WINS !"
        $game_on= false
    else                                              # ball is not in foul zone
        if [[ $player1_current_points == 0] && [ $player1_current_points == 0]]
        then
            $game_on= false
            echo "IT'S A DRAW !"
        elif [ $player1_current_points == 0 ]
        then
            $game_on= false
            $winner == 2
            echo "PLAYER 2 WINS !"
        elif [ $player2_current_points == 0 ]
        then
            $game_on= false
            $winner= 1
            echo "PLAYER 1 WINS !"
        else
            $game_on= true
            $winner= 0
        fi
    fi


print_game_board
while $game_on
do
players_pick_numbers
print_game_board
check_winner
done
}



