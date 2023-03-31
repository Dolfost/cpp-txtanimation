#!/bin/bash

part1="run_animation.cpp"
part1res="run_animation_pc"

part2="run_animation_parser.cpp"
part2res="run_animation_parser_pc"

echo "Copmpiling... $part1 to $part1res"
g++ -Wc++11-extensions $part1 -o $part1res

echo "Copmpiling... $part2 to $part2res"
g++ -Wc++11-extensions $part2 -o $part2res
