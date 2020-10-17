<?php date_default_timezone_set('Asia/Tokyo');

$file = new SplFileObject('station20151215free.txt', 'r');
$file->setFlags(SplFileObject::READ_CSV | SplFileObject::SKIP_EMPTY | SplFileObject::READ_AHEAD);
$file->setCsvControl("\t");

foreach ($file as $line)
{
    $fields[] = $line;
}

$n = count($fields);
$i = 9; // fields[9]からが実データ

echo "load --table EkiPos\n";
echo "[\n";
echo "[\"_key\", \"value\", \"location\"],\n";

for( ; $i < $n; $i++ ){
    // fields[i][10]=経度,[11]=緯度,[3]=駅名,[7]=都道府県番号
    // 駅名は同一駅名があるため、行番号-駅名 という感じでキー出力する。
    // $pref_no = $fields[$i][7];
    //if(strlen($pref_no) <= 1){
    //  $pref_no = "0" . $pref_no;
    //}
    echo "[\"" . ($i - 9 + 1) . "-" . $fields[$i][3] . "\",";
    echo " \"" . $fields[$i][3] . "\",";
    echo " \"" . $fields[$i][11] . "," . $fields[$i][10] . "\"],\n";
}

echo "]\n";
