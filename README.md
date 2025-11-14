## Tests for Vega

commands:
```bash
git clone https://github.com/Zvmcevap/vega_log_test.git
```
```bash
module load OpenMPI/4.1.6-GCC-13.2.0

```bash
module load OpenBLAS/0.3.24-GCC-13.2.0
```
```bash
module load Python/3.11.5-GCCcore-13.2.0
```

```bash
pip install -r requirements.txt
```

Log levels:
- 0: **Debug**
- 1: **Info**
- 2: **Error**
- 3+: **None**

```bash
make LOG_LEVEL=<N>
```

```
sbatch run.sh
```