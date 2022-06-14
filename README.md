# tbb-matrix-multiplication
Parallel matrix multiplication using Intel TBB library. This approached achieved the fastest running time in the Parallel programming class in 2020.

## Solution

- Intel C++ compiler v18
- Second matrix was transposed
  - Enables vectorization
  - Better cache efficiency
- AVX 256 instructions
  - Doubles the speed of vectorized functions compared to the VS compiler
  - Uses 256bit registers 
- std::inner_product
  - Standard library function that is already optimized and vectorizes pretty well
- Tbb tasks achieved the best time
  - Tree like hierarchy was created with tasks

## Hardware Specification

- Windows 10
- Intel i7 9750h processor (6 cores, 12 logical) @2.56GHz (4.5GHz boost)
- Nvidia GTX1660TI
- 16GB ddr4 ram (dual channel)
- 512 GB M2-SSD

## Results

<div align="center">
<img alt="tabelar-results" align="center" width="100%" src="https://user-images.githubusercontent.com/34657562/173551346-47dbbe94-43e0-4ec9-8cfd-2f6c661b7918.png" />
  <p align="center">Ilustration 1 - Achieved results.</p>
</div>

<div align="center">
<img alt="execution-time-visualization" align="center" width="100%" src="https://user-images.githubusercontent.com/34657562/173551422-0a479fe9-31ee-4362-b13c-4386467aa073.png" />
  <p align="center">Ilustration 2 - Execution time.</p>
</div>

<div align="center">
<img alt="speedup-compared-to-serial-baseline" align="center" width="100%" src="https://user-images.githubusercontent.com/34657562/173551534-42121d22-32f4-4caf-a444-ede65f91c3ce.png" />
  <p align="center">Ilustration 3 - Speedup compared to serial baseline.</p>
</div>

