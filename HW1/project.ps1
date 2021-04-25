foreach ( $t in 1, 2, 3, 4)
{
	foreach ( $n in 1, 10, 100, 1000, 10000, 100000, 500000)
	{
		./project1.exe $t  $n
	}
}

Start-Sleep -s 100