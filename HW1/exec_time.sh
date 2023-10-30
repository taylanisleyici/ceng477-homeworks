g++ *.cpp -o raytracer -std=c++11 -O3

start_time=$(date +%s.%N)
./raytracer inputs/bunny.xml
end_time=$(date +%s.%N)
execution_time=$(echo "$end_time - $start_time" | bc)
echo "Bunny: $execution_time seconds"
start_time=$(date +%s.%N)
./raytracer inputs/car.xml
end_time=$(date +%s.%N)
execution_time=$(echo "$end_time - $start_time" | bc)
echo "Car: $execution_time seconds"
start_time=$(date +%s.%N)
./raytracer inputs/dragon_lowres.xml
end_time=$(date +%s.%N)
execution_time=$(echo "$end_time - $start_time" | bc)
echo "Dragon: $execution_time seconds"
