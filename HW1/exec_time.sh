g++ *.cpp -o raytracer -std=c++11 -O3

start_time=$(date +%s.%N)
./raytracer inputs/horse_and_mug.xml
end_time=$(date +%s.%N)
execution_time=$(echo "$end_time - $start_time" | bc)
echo "Bunny: $execution_time seconds"
