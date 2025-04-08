# g-frame

## Darāmais

### ksvaza uzrakstītais

Jāpārskata, kā nolasīt failus VS 2022 vidē caur programmu, lai varētu pilnvērtīgi izmantot shader programmas.

Uzrakstīt meshu tekstūru un krāsu sadalītāju.
1. Atdalīt visus vēlreizizmantotos verteksus, lai katrai sejai būtu savi 3 verteksi, kā sākotnējā implementācija.
2. Kā sarežģītāks variants, atdalīt un duplicēt verteksus tikai, kur vai nesakrīt tekstūras, krāsas, vai, kur tās sakrīt, bet nesakrīt tekstūras koordinātas.

Izveidot normālvirzienu sarēķinātāju meshiem.

Implementēt meshu ģeneratoru.
1. Trijstūru, četrstūru un citu vienkāršu figūru izveidošana.
2. Līniju zīmēšana.
3. Citas palīdzības funkcijas, lai viegli varētu strādāt ar meshiem.

Rendererim iespēja uzzīmēt wireframe režīmā meshus.

Batch sistēmai.
1. Spēt noteikt sistēmas atļauto tekstūru skaitu vienā zīmēšanas izsaukumā.
2. Izveidot, lai tas var meshus sadalīt pa zīmēšanas reizēm, kad iedots liels masīvs ar zīmejamajiem meshiem.
3. Noteikti arī spēt vienu meshu sadalīt pa vairākiem, ja tiek pārsniegts dažādo tekstūru skaits.
4. Arī loģika, lai sasaistītu shader programmu ar batch sitēmas iedalītajiem meshiem.
5. Implementēt, lai var arī noteikt, vai dažādiem meshiem gadījumā nav vienādas tekstūras, savādāk izdalītas mesha ietvaros, pārdalīt to.
6. Kautkāds veids, kā lielāko daļu no darba varētu saglabāt nākamajai zīmēšanas reizei, lai varētu pārrēķināt tikai relatīvās izmaiņas starp kadriem.

Meshiem jāpārskata loģika par aizsūtīto informāciju. Iespējams katrā verteksā jāiekļauj arī normālvirziens un tekstūras identifikators, ko varētu izmantot batch sistēma, lai iedalītu tekstūras.
