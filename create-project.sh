#/bin/bash
projectName=""
execName=""

while getopts ":p:e:" opt; do
  case $opt in
    e)
      # -e execName: captura o nome do executavel
      execName=$OPTARG
      ;;
    p)
      # -p projectName: captura o nome do projeto
      projectName=$OPTARG
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done


cp -R _base $projectName
sed -i "s/<operacao>/$execName/g" $projectName/script.c
sed -i "s/<operacao>/$execName/g" $projectName/makefile
echo "!$projectName" >> .gitignore
mv $projectName/script.c $projectName/$execName.c