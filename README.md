## effz - Effective charge program

This is effz, the effective charge program that allows one to compute
observable characteristics of many electron atoms.

Contents
--------
	
- [Intro](#intro)
- [Quick Feature Summary](#quick-feature-summary)
- [Installation](#installation)
    - [Linux system](#linuxInstall)
    - [Mac OS](#install)
- [User Guide](#user-guide)
    - [Format string help](#formatString)
    - [Occupation numbers help](#occNumsString)
- [Roadmap](#roadmap)
- [Contact](#contact)
- [License](#license)

Intro
-----

The physical details of the effective charge program are discussed in
the publication in [Journal of Physics B][jphysb] or in
[ArXiv][arxiv].

Quickly summarizing, every atom of the periodic table is characterized
with the effective charge *z_eff*, which is used as a free parameter
in the [hydrogen like wave functions][wikiHydrogen]. Since the basis
is [complete and orthogonal][wikiBasisSet] it allows one to construct
regular perturbation theory. The **fully** analytical zeroth-order
approximation provides a good accuracy of about 5% for observable
characteristics for the whole periodic table. Thus it can completely
substitute the [Thomas-Fermi][wikiThomasFermi] model.

Quick Feature Summary
-----

The effective charge program can compute energies, densities and
scattering factors from a set of occupation numbers and a charge. As
of version 0.1 only the zeroth-order approximation is supported.

In the zeroth-order approximation densities and scattering factors are
computed analytically through sympy. The energies are computed
numerically.

Installation
------------

The effz program relies on a number of dependencies [*gsl*][gsl],
Intel [*tbb*][tbb] and python [*SciPy*][scipy] stack (in particular
[*sympy*][sympy] and [*matplotlib*][matplotlib]).

### Linux system

#### Python

The program is tested under **python3** and was not tested against
**python2**. 

The program relies on `python3-config` for obtaining information about
python3 environment available on the system. If `python3-config` is
not available consider to run `configure -h` and manually setup
variables related to python3. To check that `python3-config` is
available execute in terminal

```
python3-config --help
```

###### SciPy stack

If `python3` and `pip3` are available on the system the simplest way to install the SciPy is by issuing the command

```
pip3 install --user numpy scipy matplotlib sympy
```

which will download and install into the user directory the required
programs. More details together, with other ways are given
[here][scipyInstall].

In order to check for a working sympy version one can start `python3`
by typing `python3` in a terminal and execute the following script


```python
from sympy import *
init_printing()
x=symbols('x')
pprint(1/sqrt(x))
```

If after executing this code you would see nicely printed 1/√x then everything is working properly.

#### GSL

The [GNU Scientific library][gsl] can be easily installed via package manager

Ubuntu/Debian

```
sudo apt-get install gsl-bin libgsl-dev
```

Fedora/Cent Os

```
yum install gsl gsl-devel
```

Arch

```
pacman -Sy gsl
```

Gentoo

```
emerge --ask sci-libs/gsl
```

#### Intel tbb

The program relies on [Intel threading building blocks][tbb] for its multi-threaded operation.

Ubuntu/Debian

```
sudo apt-get install libtbb-dev
```

Fedora/Cent Os

```
yum install tbb tbb-devel
```

Arch

```
pacman -Sy intel-tbb
```

Gentoo

```
emerge --ask dev-cpp/tbb
```

#### [Install effz](#installeffz)

After all dependencies have been installed download from
[releases][effztar] and extract the program

```
tar -xzf effz-0.1.tar.gz
```

```
cd effz-0.1
```

and issue the standard unix triple. However, it is recommended not to install under the root. Consequently, it is better to provide the prefix option for configure like `$HOME/local` for example.

```
./configure --prefix="$HOME/local"
```

```
make
```

```
make install
```

Then the program will be available under the path `$HOME/local/bin/effz`.

Another usefull option is to specify the `--with-effz-home` variable in `configure`. During the first launch the program generates some databases and by default they are stored under `$HOME/.config/effz`. So the option `--with-effz-home=prefix` overrides the standard path `$HOME/.config` to the one provided by `prefix`.

The additional installation options are available through `configure -h`.

### Mac OS

The simplest way to install the program on Mac OS is to use
[homebrew][homebrew].

Then execute in terminal

```
pip3 install numpy scipy matplotlib sympy
```

```
brew install gsl
```

```
brew install tbb
```

and follow the instructions from [Install effz](#installeffz) above.

### Windows

TODO the support for Windows system is shifted to the 0.1.x version.

User Guide
----------

##### Format string help

In order to perform computation the parameters should be given in a
form of a format string.

The format string can be specified in the file or in the standard
input, i.e., terminal window or in the interactive mode of a program.

The format string looks like:


	-z Z1, Z2, ...  -f OCC_NUMS_FORMAT -v OCC_NUMS1, OCC_NUMS2, ...; REPEAT_FORMAT_STRING


**EXPLANATION.**

* -z specifies the charges.  All arguments except of Z1 are optional.
Z1, Z2, ...  are numbers.

* -f specifies the format of the occupation numbers and can be one of
the following n, N, i, O, o for more information read OCC_NUMS_FORMAT
help.

* -v specifies the occupation numbers in the format
  OCC_NUMS_FORMAT. All arguments except of OCC_NUMS1 are optional.

* The semicolon at the end separates format strings, since multiple
strings can be provided.  All format strings except of the first one
are optional.


**RESULTS OF THE COMPUTATION.**

Provided the format string above, the program computes for all charges
Z1, Z2, ...  corresponding characteristics. Thus yielding num_charges *
num_occ_nums values, by default the values are output to the screen.

If to the format string before the ; the flag -o is provided (**will be released as of version 0.1.x**) with a
file path, i.e.,


	-z Z1, Z2, ...  -f OCC_NUMS_FORMAT -v OCC_NUMS1, OCC_NUMS2, ...  -o PATH_TO_OUTPUT_FILE;
	

then the output is saved into the file.

##### Occupation numbers help

Occupation numbers are specified in one of the following formats:

* n --- numerical format, e.g., 5 using occupation numbers of the B
element.


* N --- named format, e.g.  He using occupation numbers of the He
element.


* i --- ionization format, i.e., element name + roman number.  e.g.  Ne II. This means occupation numbers for Ne minus 1 electron.


* O --- occupation numbers and an element name format i.e., element name + {{n,l,m,ms}}, e.g.  B {{2,1,0,1}} or B {{2,1,0,1},{2,1,1,-1}}.
Note: more than two internal brackets are possible.  In this case we
take Boron occupation numbers and append the corresponding occupation
numbers in brackets.


* o --- a set of occupation numbers format e.g.  {{1,0,0,1},{1,0,0,-1}}


**IMPORTANT**: If there are identical occupation numbers present the
energy is equal to zero.

**IMPORTANT**: The occupation numbers are sorted in the order of
increasing n, l, m, ms.  This means that we fill m form -l to l and ms
from -1 to 1


Roadmap
----------

v0.1 The first release contains only the zeroth-order approximation and not all functions of the planned v0.1 release have been implemented. This includes reading format strings from the command line and from file. 
   
Initial design of the program consists of separation of the code for the utility functions and the scientific part. However, in this release the actual separation for the program and the library is not performed and is planned for the 0.1.x release. Consequently, the scientific part can be easily incorporated into third party PIC codes for simulating ionization.
	
* v0.1.x will contain the implementation of the above described functions from v0.1 and the separation of the code into the library and the program.

* v0.2 will contain the implementation of the second-order single-electron excitation correction. In addition, the dependence on *gsl* is planned to be dropped.

* v0.2.x will contain the implementation of the second-order multi-electron excitation correction and implementation of the calculation of the matrix elements.

* v0.3 will contain the implementation of the third-order single-electron excitation correction.

Contact
-------

If you have questions about the program or need help, please write me
an email <olegskor@gmail.com>.

If you have bug reports or feature suggestions, please use the [issue
tracker][issueTracker].

The latest version of the program is available at
<https://github.com/tupos/effz>.

The author's homepage is <https://github.com/tupos>.

License
-------

effz is free software, you can redistribute it and/or modify it under
the terms of [Apache License 2.0][apache].

Unless required by applicable law or agreed to in writing, Licensor
provides the Work (and each Contributor provides its Contributions) on
an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
express or implied, including, without limitation, any warranties or
conditions of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR
A PARTICULAR PURPOSE.

© 2018 effz authors

[apache]: https://www.apache.org/licenses/LICENSE-2.0
[arxiv]: https://arxiv.org/abs/1701.04800
[jphysb]: http://iopscience.iop.org/article/10.1088/1361-6455/aa92e6/meta
[wikiHydrogen]: https://en.wikipedia.org/wiki/Hydrogen_atom
[wikiBasisSet]: https://en.wikipedia.org/wiki/Orthonormal_basis
[wikiThomasFermi]: https://en.wikipedia.org/wiki/Thomas%E2%80%93Fermi_model
[issueTracker]: https://github.com/tupos/effz/issues
[gsl]: https://www.gnu.org/software/gsl/
[tbb]: https://www.threadingbuildingblocks.org/
[scipy]: https://www.scipy.org/about.html
[scipyInstall]: https://www.scipy.org/install.html
[sympy]: http://www.sympy.org/de/index.html
[matplotlib]: https://matplotlib.org/index.html
[homebrew]: https://brew.sh/
[effztar]: https://github.com/tupos/effz/releases
