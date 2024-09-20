echo the name of the script is $0
echo the first argument is $1
echo a list of all arguments is $*
echo this script places the date into a temporary file echo called $1.$$
echo date > $1.$$
date
ls $1.$$
