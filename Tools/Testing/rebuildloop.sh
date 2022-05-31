i=0
while true; do
    echo Build $i
    i=$i+1
    sh Test-Gcc.sh > Build.log
    if [ $? -ne 0 ]; then
        break
    fi
    sleep 1
done
