echo "make"
make
echo "give permissions to rt"
chmod 777 ./rt
echo "delete old rt"
rm ../data/rt
echo "cp rt to new location"
cp ./rt ../data

cd ../data
./rt nonhiersphere.lua && display nonhiersphere.png

