make
start_time_full=$(date +%s.%N)
for file in inputs/*
do
    start_time=$(date +%s.%N)
    echo $file
    ./raytracer $file
    end_time=$(date +%s.%N)
    execution_time=$(echo "$end_time - $start_time" | bc)
    echo "$file: $execution_time seconds"
done
echo "Total: $(echo "$(date +%s.%N) - $start_time_full" | bc) seconds"