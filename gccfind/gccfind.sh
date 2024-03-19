#!/bin/bash
#David Gurovich 318572047
pathLength="${#0}"
if [ "$pathLength" \> "12" ];
then
cd ${0::-11}
fi
if [ -z "$2" ] ;
then
echo "Not enough parameters"
else
cd $1
if [ "$3" == "-r" ] ;
then
    FULLPATH=  pwd
    find  -name "*.out" -type f -delete
    FILELIST="$(grep -ril --include="*.c" "$2" . || grep -ril --include="*.c" "$2!" . || grep -ril --include="*.c" "$2." . )"
    #echo "$FILELIST"
    for filePath in $FILELIST
    do
        IFS='/' #setting / as delimiter  
        read -ra arrDir <<< $filePath; fileName=${arrDir[-1]}; #reading str as an array as tokens separated by IFS 
        
        #echo "${filePath::$diff}"
        iterations=${#arrDir[@]}
        realIterations=$(($iterations-2))
        if [ "$realIterations" == "0" ];
        then
            gcc -w "$fileName" -o "${fileName::-2}.out"
        else
        for j in $(seq $realIterations); do
	        cd ${arrDir[$j]}
        done
        #echo "$fileName"
        gcc -w "$fileName" -o "${fileName::-2}.out"
        for j in $(seq $realIterations); do
            cd ..
        done
        fi
        #gcc -w "$filePath" -o "${filePath::-2}.out";
         
    done
else
rm *.out || pwd
for i in * 
do
    if test -f  "$i";
    then
        if  [ "${i: -2}" == ".c" ];
        then
            if  grep -Fqi "$2"  "$i" || grep -Fqi "$2."  "$i" || grep -Fqi "$2!"  "$i" ;
                then gcc -w "$i" -o "${i::-2}.out";
            fi
        fi
    fi
done
fi
 
fi