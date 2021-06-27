#!/bin/bash
racket1=►
racket2=◄
length=4;
points1=0
points2=0

column=$(( `tput cols`-2 ))
line=$(( `tput lines`-1 ))

starting1=$(( line/2 ))
starting2=$(( line/2 ))

draw_board() {
    case "$1" in 
        1)
            (( starting1 += $2 ))
            (( starting1 = starting1 < 1 ? 1 : starting1 > line-length ? line-length : starting1 ));;
        2) 
            (( starting2 += $2 ))
            (( starting2 = starting2 < 1 ? 1 : starting2 > line-length ? line-length : starting2 ));;

    esac

    for value in `seq $line`; do
    	if (( value < line-1 )); then
        echo -en "\033[$(( value+1 ));$(( column/2 ))H|"
        echo -en "\033[$(( value+1 ));$(( column + 1 ))H|"
        echo -en "\033[$(( value+1 ));0H|"

        echo -en "\033[${value};2H "
        echo -en "\033[${value};${column}H "
        fi
    done

    for value in `seq $length`; do
        echo -en "\033[$(( starting1 + value ));0H$racket1"
        echo -en "\033[$(( starting2 + value ));${column}H$racket2"
    done

    echo -en "\033[2;$(( column/2 - 15 ))HPlayer 1: $points1"
    echo -en "\033[2;$(( column/2 + 6 ))HPlayer 2: $points2"

    for value in `seq $column`; do
    	if (( value < column-2 )); then
        echo -en "\033[0;$(( value+2 ))H-"
        fi
        echo -en "\033[${line};$(( value+1 ))H_"
    done
}
tput civis
tput clear
draw_board

max_speed=3
vel_x=-1
vel_y=1
pos_x=$(( column/2 ))
pos_y=$(( line/2 ))

while [[ $q != q ]]; do
    echo -e "\033[${pos_y};${pos_x}H●"
    read -n 1 -s -t 0.05 q

    if (( pos_x < column/2 )); then
        case "$q" in
            a)  draw_board 1 -2;;
            z)  draw_board 1 2;;
        esac
    elif (( pos_x > column/2 )); then 
        case "$q" in
            k)  draw_board 2 -2;;
            m)  draw_board 2 2;;
        esac
    fi

    if (( pos_x >= column/2 - 1 && pos_x <= column/2 + 1 )); then
        draw_board
    fi

    echo -e "\033[${pos_y};${pos_x}H "
    
    if (( pos_x + vel_x >= column || pos_x + vel_x <= 1 )); then 
        (( vel_x = - vel_x ))
    fi
    if (( pos_y <= 1 || pos_y >= line )); then 
        (( vel_y = - vel_y ))
        draw_board
    fi

    if (( pos_x > column-1+vel_x )); then
        if (( pos_y >= starting2 - 1 && pos_y <= starting2 + length + 1 )); then
            if (( pos_y < starting2 + length/2 )); then 
                vel_y=-1
                vel_x=-1
            elif (( pos_y > starting2 + length/2 )); then 
                vel_y=1
                vel_x=-1
            else 
                vel_y=0
                vel_x=-2
            fi
        else 
            (( points1 += 1 ))
            pos_x=$(( column/2 ))
            pos_y=$(( line/2 ))
            vel_x=-1
            vel_y=1
            draw_board
        fi
    fi      
    
    if (( pos_x < 2+vel_x )); then
        if (( pos_y >= starting1 - 1 && pos_y <= starting1 + length + 1 )); then
            if (( pos_y < starting1 + length/2 )); then 
                vel_y=-1
                vel_x=1
            elif (( pos_y > starting1 + length/2 )); then 
                vel_y=1
                vel_x=1
            else 
                vel_y=0
                vel_x=2
            fi
        else
            (( points2 += 1 ))
            pos_x=$(( column/2 ))
            pos_y=$(( line/2 ))
            vel_x=1
            vel_y=1
            draw_board
        fi
    fi

    (( pos_x += vel_x ))
    (( pos_y += vel_y ))
done
tput cnorm
