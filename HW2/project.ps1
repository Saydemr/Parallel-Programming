g++ project2.cpp -o proj2 -lm -fopenmp

foreach ( $t in 1, 2, 3, 4)
{
	foreach ( $n in 10, 25, 50, 100, 250, 500, 1000)
	{
		./proj2 $t  $n
	}
}

Start-Sleep -s 100