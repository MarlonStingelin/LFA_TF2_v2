# LFA_TF2_v2

Pseudo-código:
  Quantas linhas devem funcionar no dia? (Max 4) //num_linhas
  	Qual o máximo de veículos que pode ser produzido no dia? //max_prod
  	Qual o nome do Modelo? //nome_modelo
  	Quantas caracteristicas o modelo apresenta? (0 || 1 || 2) //num_caract
  	se num_caract = 1
  		Qual o nome da caracteristica? //caract_1
  	se num_caract = 2
  		Qual o nome da primeira caracteristica? //caract_1
  		Qual o nome da segunda caracteristica? //caract_2
  	Qual a chance de erro na montagem do veículo? (0 até 100) //chance_falha 
  	gera a fita baseada no nun_caract e chance_falha //fita_siglas
  	separa as siglas e da número a elas //fita_o
  	leitura da fita por meio do automato com pilha especifico da num_caract
  		gera pilha //pilha_result
  	  para cada letra T ou F empilhada "i=i++"
  desempilha a pilha_result //result
  	reorganiza result //result_o
  	combina os números de result_o e fita_o
  	gera relatorio
  repete enquanto num_linhas != 0

Esse programa vai "simular" uma linha de produção de automóveis através de um autômato com pilha, a ao fim vai gerar um relatório sobre os veículos produzidos pela montadora no dia. Podem haver 4 linhas de produção de diferentes modelos funcionando no mesmo dia, eles ainda pode apresentar 0 ou 1 ou 2 características. Os veículos serão dentro do programa tratados com um sigla essa sigla tem um padrão sendo a primeira letra o modelo do veiculo e sua letras seguintes as possíveis características diferenciais do modelo e a ultima letra indica se houveram falhas na montagem do veiculo. 
ex:
  modelos:a caracteristicas:x,y sem_caracteristica:n  sem_falhas:v com_falha:f
  anv - modelo a sem caracteristicas montado com sucesso
  axv - modelo a com caracteristica x montado com sucesso
  axyv - modelo a com caracteristicas x e y montado com sucesso
  annf - modelo a sem caracteristicas x e y com falha na montagem
  anyf - modelo a com caracteristica y e sem a cracteristica x com falha na montagem 
  ...

o programa vai receber um valor inteiro chamado de num_caract e deve gerar um número aleatório de siglas formadas por uma letra inicial fornecida pelo programa zero ou mais letras que vão ser controladas pelo num_caract e uma letra final que pode ser "v" ou "f", ao gerar um sigla ele deve adicionar "|" ou "." sendo "|" usado para quando existem mais siglas geradas adiante e "." significa que chega ao fim do código. o num_caract é limitadas de 0 a 2, caso sejam 0 ele só gera a primeira e ultima letra, se for igual 1, ele terá uma letra a mais após a primeira sendo essa chamada de característica podendo ser "x" ou "n", já no caso de num_caract ser 2 ele vai gerar siglas com outra letra anterior a a ultima, sendo essa letra "y" ou "n". Quanto a ultima letra ela tem uma porcentagem fornecida pelo sistema com a chance de ser "f", mas por padrão ele recebe "v". Vale lembrar que o número de siglas é aleatório (mas limitado de 1 a 10 siglas).
ex:     
  "av|av|av|av." (caso num_caract = 0, letra inicial = a, chance da ultima letra ser "f" = 0%) 
	"av|av|af|af." (caso num_caract = 0, letra inicial = a, chance da ultima letra ser "f" = 50%)
	"anv|axv|axf|anf|axv." (caso num_caract = 1, letra inicial = a, chance da ultima letra ser "f" = 40%)
	"anyv|axyv|axnf|annf|axyv." (caso num_caract = 2, letra inicial = a, chance da ultima letra ser "f" = 40%)
	"anyf|axyf|axnf|annf|axyf." (caso num_caract = 2, letra inicial = a, chance da ultima letra ser "f" = 100%)
 essa saìda é chamada fita_siglas

Após isso o programa deve separar as siglas contidas no fita e salvar elas associando a um número, isso facilitara a escrita do relatorio
ex: 
  sigla_i|sigla_i+1|sigla_i+1+1. gera: 1sigla_i 2sigla_i+1 3sigla_i+1+1
  anv|axv|axf. vai gerar 1anv 2axv 3axf
essa saída pode ser chamada de fita_o

o programa vai rodar o fita_siglas (pedidos de montagem das siglas) essa fita sera lida pelo automato com pilha (a principio a ideia era gerar a pilha dependendo do número de caracteristicas do veículo) automato pilha é montado pelas seguintes caracteristicas "proximo_estado = estado_atual ( letra_lida_da_fita , letra_a_ser_empilhada , letra_a_ser_desempilhada )" .
quando o ele recebe a letra "&" que simboliza o vazio  
ex: q1 = q0 ( a , A , & ) ou seja estando no estado q0 e le na fita "a" empilha "A" e não desempilha nada e pasa para o estado q1
    q2 = q1 ( b , & , B ) ou seja estando no estado q1 e le na fita "b" não empilha nada e desempilha "B" e passa para o estado q2

No caso de num_caract = 0
q0 é o estado inicial e F é o estado final
qa = q0 ( a , & , & )

qv = qa ( v , "i", & )
qf = qa ( f , "i", & )

qa = qv ( | , T , & ) 
qa = qf ( | , F , & )

F = qv ( . , T , & )
F = qf ( . , F , & )

No caso de num_caract = 1
q0 é o estado inicial e F é o estado final
qa = q0 ( a , & , & )

qx = qa ( x , "i", & )
qn1 = qa ( n , "i", & )

qv = qx ( v , & , & )
qf = qx ( f , & , & )
qv = qn1 ( v , & , & )
qf = qn1 ( f , & , & )

qa = qv ( | , T , & ) 
qa = qf ( | , F , & )

F = qv ( . , T , & )
F = qf ( . , F , & )

No caso de num_caract = 2
q0 é o estado inicial e F é o estado final
qa = q0 ( a , & , & )

qx = qa ( x , "i", & )
qn1 = qa ( n , "i", & )

qy = qx ( y , & , & )
qn2 = qx ( n , & , & )
qy = qn1 ( y , & , & )
qn2 = qn1 ( n , & , & )

qv = qy ( v , & , & )
qf = qy ( f , & , & )
qv = qn2 ( v , & , & )
qf = qn2 ( f , & , & )

qa = qv ( | , T , & ) 
qa = qf ( | , F , & )

F = qv ( . , T , & )
F = qf ( . , F , & )

toda vez que a Pilha receber um novo valor podendo ser T ou F, i=i+1, i começa sendo 1. Ao fim da execusão a pilha gerada e desempilhada ficando 
num formato 
ex: 4 T 3 T 2 F 1 T 
essa saída se chama result
partindo disso o programa separa um valor e a letra seguinte dele, assim ele junta as informações da seguinte maneira
ex: 1T 2F 3T 4T 
essa saída se chama result_o 
combinando a fita_o e result_o tendo em vista que o valor que aparece em sua frente relaciona a sigla a letra da pilha
a número representa qual sera o veiculo na ordem de produção dele. Caso a sigla n tenha "x" e nem "y" o relatorio gera um "nada", caso tenha "x" e não "y" gera "caract_1", caso tenha "y" e não "x" gera "caract_2", e se tiver "x" e "y" gera "caract_1" e "caract_2"
"caracts" = "nada" || "caract_1" || "caract_2" || "caract_1" e "caract_2"
sendo T : Montagem com sucesso e sendo F : Montagem com falhas
tendo isso ele pode gerar o relatorio que contém as seguintes informações 
ex:

Produção da Linha "num_linha" :
Veiculo 1 - "sigla_i" - Modelo "nome_modelo" Caracteristicas "caracts"
Montagem com sucesso
Veículo 2 - "sigla_i+1" - Modelo "nome_modelo" Caracteristicas "caracts" 
Montagem com falhas
Veículo 3 - "sigla_i+1+1" - Modelo "nome_modelo" Caracteristicas "caracts" 
Montagem com sucesso
Veículo 4 - "sigla_i+1+1+1" - Modelo "nome_modelo" Caracteristicas "caracts"
Montagem com sucesso

no caso de ter menos veículos produzidos do que o maximo que a linha pode produzir, ele vai gerar apenas até o ultimo valor de i apresentado

no caso da linha de produção ter mais pedidos para montagem do que a linha pode produzir, ele deve so ir até o que o valor i que seja igual ao max_prod

