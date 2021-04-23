# Zhang-Suen-Thinning-Algorithm

### This is the implementation of the Zhang-Suen thinning algorithm. It's implemented with two different ways. A serial and a parallel version.



![before](/examples/A/A.png) &nbsp; &nbsp; ![after](/examples/A/A_skeleton.png)

| Processes     | Execution Time (s) |Acceleration (Sp=Ts/Tp)|Efficiency (Sp/p)  | Communication Cost (s)|
|:-------------:|:------------------:|:---------------------:|:-----------------:|:---------------------:|
| 1             | 0.1272912          | 1                     | 1                 | -                     |
| 2             | 0.1172022          | 1.08                  | 0.54              | 0.0053226             |
| 4             | 0.085999           | 1.48                  | 0.37              | 0.0154506             |
| 8             | 0.2005016          | 0.634                 | 0.079             | 0.1286832             |
| 16            | 0.4238256          | 0.3                   | 0.018             | 0.3372468             |
</br>

![before](/examples/B/B.png) &nbsp; &nbsp; ![after](/examples/B/B_skeleton.png)

| Processes     | Execution Time (s) |Acceleration (Sp=Ts/Tp)|Efficiency (Sp/p)  | Communication Cost (s)|
|:-------------:|:------------------:|:---------------------:|:-----------------:|:---------------------:|
| 1             | 0.0727708          | 1.03                  | 1.03              | -                     |
| 2             | 0.064187           | 1.13                  | 0.565             | 0.002727              |
| 4             | 0.0805224          | 0.903                 | 0.225             | 0.0066016             |
| 8             | 0.2064952          | 0.352                 | 0.044             | 0.1831272             |
| 16            | 0.4732442          | 0.153                 | 0.009             | 0.4098266             |
</br>

![before](/examples/bin_lines/bin_lines.png) &nbsp; &nbsp; ![after](/examples/bin_lines/bin_lines_skeleton.png)

| Processes     | Execution Time (s) |Acceleration (Sp=Ts/Tp)|Efficiency (Sp/p)  | Communication Cost (s)|
|:-------------:|:------------------:|:---------------------:|:-----------------:|:---------------------:|
| 1             | 2.1640772          | 0.98                  | 0.98              | -                     |
| 2             | 1.144749           | 1.89                  | 0.945             | 0.0409786             |
| 4             | 0.7248446          | 2.98                  | 0.748             | 0.1296998             |
| 8             | 0.6203674          | 3.488                 | 0.436             | 0.2193812             |
| 16            | 0.7055856          | 3.067                 | 0.191             | 0.3879998             |

## References

The algorithm is based on this [paper](https://d1wqtxts1xzle7.cloudfront.net/32991163/thiniing_algo.pdf?1392347248=&response-content-disposition=inline%3B+filename%3DRESEARCH_CONTRIBUTIONS_Image_Processing.pdf&Expires=1619206421&Signature=Z7bGRaN~6EHuYZdS0kGlJ1XDnlG7UsrEs-w7GGjjlRBflE3Ik2oGGy78XjSFdNEeUYHEEzzhlLPbnDoXyo~bhTWKYdZukSV7lngACt3v6L0L4nuFXLFvHihchqqy7JkMBJE3M3ki4fJX38hKQdMQuYOll8m3dArZJ11GGtq-dtlDjxzAyHlOmmWXaQX39fYAa0Dpb4KlCLF3USC3tJoB~EXrlNRaKGupMIWsMWyXvHb1vfzNAWFDlzAO1Had1F5A-GiEQgHCoE~gJkQk0jVdwM-ZIQM7PUPoW3hly2cfYp2qsF8tdMSJn9quMrgWWFWyITyjgWvxvmLSU3ULRuWBQg__&Key-Pair-Id=APKAJLOHF5GGSLRBV4ZA)
