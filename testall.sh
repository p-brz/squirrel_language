# Limpa arquivo ou cria se não existe.
echo "" >  testresult.txt

# Testa todos os arquivos em example_programs
for file in example_programs/* ; 
do 
  echo "Teste $file " $'\n' "============== " $'\n' >> testresult.txt; 
  ./build/squirrel < "$file" >> testresult.txt; 
done
