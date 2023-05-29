# ppcf
Short program for calculating the pair distribution functions of atomic systems from their coordinates
A program kap n db konfigurációt, amelyeknek egyesével kiszámítja a parciális párkorrelációs függvényeit. Megszámolja, hogy egy atom r sugarú környezetében hány atom van, majd ezt a számot elosztja egy r sugarú gömb térfogatával, ezzel megkap egy lokális számsűrűséget. Mindezt elvégzi minden atomra megadott típusú. A lokális számssűrűséget (db/térfogat) elosztja a globális számsűrűséggel, ez lesz a gr. Ez az érték azonban gyorsan kiátlagolódna, azaz tartana 1-hez, így érdemes a teret binekre osztani, és csak az r+dr és r közötti térrészben számolni az atomokat.
Az atom egy osztályba vannak foglalva, amely tárolja az x y z koordinátáikat, valamint az atom típusát. Az atomok vagy egy tömbbe vagy egy dobozba lesznek tárolva, attól függően, hogy szükséges-e a doboz absztrakció. Az egyes atomok a fájlokban 1-1 sort foglalnak el, a << operátor fogja őket beolvasni és az hívja a constructort. A beolvasott fájloknak lesz fejléce is, ezeket először le kell szedni.
A kiszámított r - gr értékeket szintén tárolódhatnak saját osztályban, amellyel szintén << operátorral ki lehet őket íratni fájlba. Ez az osztály tartalmazna r-t: az aktuális bin (r+dr - r intervallum) közepét, valamint a parciális gr-eket pl. víz esetén grHH grHO(OH ugyanaz) grOO
A feladatban a nehézség nem az implemntáció, hanem a hatékonyság, létezik már gyári verzió is, ha az nagyságrendekkel gyorsabb, akkor a kód értelmét veszti.

Felhasznált könyvtárak:
https://github.com/PCRE2Project/pcre2/releases

