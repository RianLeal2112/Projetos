#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
typedef struct{
    GtkWidget *nome_label;
    GtkWidget *valor_label;
    GtkWidget *quantidade_label;
    GtkWidget *lista_botao; //botao de mandar pro carrinho
    GtkWidget *botao_remover;
    char nome[50];
    float valor;
    int quantidade;
    int id;
    int usabilidade;
}produto;
typedef struct{
    GtkWidget *janela;
    //cadastro
    GtkWidget *cad;
    GtkWidget *box_cad;
    GtkWidget *cad_nome;//entrada
    GtkWidget *cad_valor;//entrada
    GtkWidget *cad_quantidade;//entrada
    GtkWidget *cad_label;
    //lista
    GtkWidget *lista; //notebook
    GtkWidget *lista_prod[50]; //grid individual de cada prouto ||nome do produto||\n |valor|quantidade|
    produto produtos[50];
    //carrinho
    produto produtos_c[50];
    GtkWidget *carrinho;
    GtkWidget *carrinho_grid;
    GtkWidget *valor;
    GtkWidget *lista_carrinho[50]; //grid individual de cada prouto ||nome do produto||\n |valor|quantidade|
    //acesso
    GtkWidget *entrada;
    GtkWidget *botao_verificar;
    GtkWidget *botao_visibilidade;
    GtkWidget *acesso;
    int alt;
    char senha[10];

    GtkWidget *notebook;
    int carrinho_id[50];
    int carrinho_c;
    int id;
    float valor_total;
}programa;

void calcular(programa *ctx){
    ctx->valor_total = 0;
    for(int c = 0; c < ctx->carrinho_c; c++){
        ctx->valor_total = ctx->valor_total + ctx->produtos_c[c].valor*ctx->produtos_c[c].quantidade;
    }
    char val[50];
    sprintf(val, "R$ %.2f", ctx->valor_total);
    gtk_label_set_text(GTK_LABEL(ctx->valor), val);
    gtk_widget_show_all(ctx->janela);
}
void atualizar_quant(programa *ctx, int p, int array){
    char quant[20];
    if(array == 1){
        sprintf(quant, "%d",ctx->produtos[p].quantidade);
        gtk_label_set_text(GTK_LABEL(ctx->produtos[p].quantidade_label), quant);
    }else if(array == 2){
        sprintf(quant, "%d",ctx->produtos_c[p].quantidade);
        gtk_label_set_text(GTK_LABEL(ctx->produtos_c[p].quantidade_label), quant);
    }
    gtk_widget_show_all(ctx->janela);
}

static void verificar_f(GtkWidget *widget, gpointer data){
    programa *ctx = (programa *)data;
    const char *entry = gtk_entry_get_text(GTK_ENTRY(ctx->entrada));
    char entrada[50];
    sprintf(entrada, "%s", entry);
    int verf = strncmp(ctx->senha, entrada, 6);
    if(verf == 0){
        gtk_notebook_set_current_page(GTK_NOTEBOOK(ctx->notebook), 1);
        gtk_widget_show(ctx->box_cad);
    }else{
        gtk_notebook_set_current_page(GTK_NOTEBOOK(ctx->notebook), 2);
    }
}
static void voltar_f(GtkWidget *widget, gpointer data){
    programa *ctx = (programa *)data;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(ctx->notebook), 2);
}
static void visibilidade(GtkWidget *widget, gpointer data){
    programa *ctx = (programa *)data;
    if(ctx->alt % 2 == 0){
        gtk_entry_set_visibility(GTK_ENTRY(ctx->entrada), FALSE);
    }else{
        gtk_entry_set_visibility(GTK_ENTRY(ctx->entrada), TRUE);
    }
    ctx->alt++;
}
static void apagar(GtkWidget *widget, gpointer data){
    produto *clicado = (produto *)data;
    programa *ctx = (programa *)g_object_get_data(G_OBJECT(widget), "ctx");
    int p = clicado->id, p2;
    for(int c = 0; c < ctx->carrinho_c; c++){
        if(ctx->produtos_c[c].id == p){
            p2 = c;
        }
    }
    if(clicado->quantidade > 1){
        clicado->quantidade--;
        ctx->produtos[p].quantidade++;
        atualizar_quant(ctx, p2, 2);
    }else if(clicado->quantidade == 1){
        clicado->usabilidade = 1;
        clicado->valor = 0;
        clicado->quantidade = 0;
        ctx->produtos[p].quantidade++;
        gtk_label_set_text(GTK_LABEL(clicado->nome_label), "PRODUTO REMOVIDO");
        gtk_label_set_text(GTK_LABEL(clicado->valor_label), " ");
        gtk_label_set_text(GTK_LABEL(clicado->quantidade_label), " ");
        gtk_button_set_label(GTK_BUTTON(clicado->botao_remover), "REMOVIDO");
    }
    atualizar_quant(ctx, p, 1);
    calcular(ctx);
}
static void adicionar_produto(GtkWidget *widget, gpointer data){
    produto *clicado = (produto *)data;
    programa *ctx = (programa *)g_object_get_data(G_OBJECT(widget), "ctx");
    int k = 0, p2;
    int p = clicado->id;
    if(clicado->quantidade > 0){
        for(int c = 0; c < ctx->carrinho_c; c++){
            if(p == ctx->carrinho_id[c]){
                k++;
                p2 = c;
            }
        }
        if(k == 0){
            ctx->carrinho_id[ctx->carrinho_c] = p;
            //transferencia de dados
            char valor[20], quantidade[20];
            strcpy(ctx->produtos_c[ctx->carrinho_c].nome, clicado->nome);
            ctx->produtos_c[ctx->carrinho_c].valor = clicado->valor;
            ctx->produtos_c[ctx->carrinho_c].quantidade = 1;
            ctx->produtos_c[ctx->carrinho_c].nome_label = gtk_label_new(ctx->produtos[ctx->carrinho_c].nome);
            sprintf(valor, "R$ %.2f", ctx->produtos[ctx->carrinho_c].valor);
            ctx->produtos_c[ctx->carrinho_c].valor_label = gtk_label_new(valor);
            sprintf(quantidade, "%d", ctx->produtos_c[ctx->carrinho_c].quantidade);
            ctx->produtos_c[ctx->carrinho_c].quantidade_label = gtk_label_new(quantidade);
            ctx->produtos_c[ctx->carrinho_c].id = p;
            ctx->produtos_c[ctx->carrinho_c].nome_label = gtk_label_new(ctx->produtos_c[ctx->carrinho_c].nome);

            ctx->produtos_c[ctx->carrinho_c].botao_remover = gtk_button_new_with_label("REMOVER");
            g_signal_connect(ctx->produtos_c[ctx->carrinho_c].botao_remover, "clicked", G_CALLBACK(apagar), &ctx->produtos_c[ctx->carrinho_c]);
            g_object_set_data(G_OBJECT(ctx->produtos_c[ctx->carrinho_c].botao_remover), "ctx", ctx);


            //visualização do produto no carrinho
            gtk_grid_attach(GTK_GRID(ctx->carrinho), ctx->produtos_c[ctx->carrinho_c].nome_label, 0, ctx->carrinho_c, 1, 1);
            gtk_grid_attach(GTK_GRID(ctx->carrinho), ctx->produtos_c[ctx->carrinho_c].valor_label, 1, ctx->carrinho_c, 1, 1);
            gtk_grid_attach(GTK_GRID(ctx->carrinho), ctx->produtos_c[ctx->carrinho_c].quantidade_label, 2, ctx->carrinho_c, 1, 1);
            gtk_grid_attach(GTK_GRID(ctx->carrinho), ctx->produtos_c[ctx->carrinho_c].botao_remover, 3, ctx->carrinho_c, 1, 1);
            //atualiza a quantidade de produtos na loja
            clicado->quantidade--;
            atualizar_quant(ctx, p, 1);
            ctx->carrinho_c++;
        }else if(k != 0 && ctx->produtos_c[p2].usabilidade == 0){
            clicado->quantidade--;
            ctx->produtos_c[p2].quantidade++;
            atualizar_quant(ctx, p, 1);
            atualizar_quant(ctx, p2, 2);
        }else{
            char valor[50];
            strcpy(ctx->produtos_c[p2].nome, clicado->nome);
            ctx->produtos_c[p2].valor = clicado->valor;
            sprintf(valor, "R$ %.2f", ctx->produtos_c[p2].valor);
            ctx->produtos_c[p2].quantidade = 1;
            clicado->quantidade--;
            ctx->produtos_c[p2].usabilidade = 0;
            gtk_label_set_text(GTK_LABEL(ctx->produtos_c[p2].nome_label), ctx->produtos_c[p2].nome);
            gtk_label_set_text(GTK_LABEL(ctx->produtos_c[p2].valor_label), valor);
            gtk_label_set_text(GTK_LABEL(ctx->produtos_c[p2].quantidade_label), "1");
            gtk_button_set_label(GTK_BUTTON(ctx->produtos_c[p2].botao_remover), "REMOVER");
            atualizar_quant(ctx, p, 1);
            gtk_widget_show_all(ctx->janela);
        }
        calcular(ctx);
    }
}
static void cadastrar_produto(GtkWidget *widget, gpointer data){
    programa *ctx = (programa *)data;
    
    //receber e armazenar
    const char *nome = gtk_entry_get_text(GTK_ENTRY(ctx->cad_nome));
    const char *valor = gtk_entry_get_text(GTK_ENTRY(ctx->cad_valor));
    const char *quantidade = gtk_entry_get_text(GTK_ENTRY(ctx->cad_quantidade));
    char valorr[50];
    float val = atof(valor);
    int quant = atoi(quantidade);
    sprintf(valorr, "R$ %.2f  ", val);
    sprintf(ctx->produtos[ctx->id].nome, "%s", nome);
    ctx->produtos[ctx->id].id = ctx->id;
    ctx->produtos[ctx->id].valor = val;
    ctx->produtos[ctx->id].quantidade = quant;

    //gerar labels e grid
    ctx->lista_prod[ctx->id] = gtk_grid_new();
    ctx->produtos[ctx->id].nome_label = gtk_label_new(ctx->produtos[ctx->id].nome);
    ctx->produtos[ctx->id].valor_label = gtk_label_new(valorr);
    ctx->produtos[ctx->id].quantidade_label = gtk_label_new(quantidade);

    ctx->produtos[ctx->id].lista_botao = gtk_button_new_with_label("comprar");
    g_signal_connect(ctx->produtos[ctx->id].lista_botao, "clicked", G_CALLBACK(adicionar_produto), &ctx->produtos[ctx->id]);
    g_object_set_data(G_OBJECT(ctx->produtos[ctx->id].lista_botao), "ctx", ctx);

    //adicionar ao grid individual
    gtk_grid_attach(GTK_GRID(ctx->lista_prod[ctx->id]), ctx->produtos[ctx->id].nome_label, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(ctx->lista_prod[ctx->id]), ctx->produtos[ctx->id].valor_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(ctx->lista_prod[ctx->id]), ctx->produtos[ctx->id].quantidade_label, 1, 1, 1, 1);
    //posicionamento na grid da pagina
    gtk_grid_attach(GTK_GRID(ctx->lista), ctx->lista_prod[ctx->id], 0, ctx->id + 1, 1, 1);
    gtk_grid_attach(GTK_GRID(ctx->lista), ctx->produtos[ctx->id].lista_botao, 1, ctx->id + 1, 1, 1);

    gtk_label_set_text(GTK_LABEL(ctx->cad_label), "produto cadastrado!");
    gtk_widget_show_all(ctx->janela);
    ctx->id++;
}
int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    programa ctx;
    for(int c = 0; c < 50; c++){
        ctx.produtos_c[c].usabilidade = 0;
    }
    
    ctx.alt = 1;
    ctx.id = 0;
    ctx.carrinho_c = 0;
    ctx.valor_total = 0;
    strcpy(ctx.senha, "ADM123");
    ctx.janela = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(ctx.janela), 400, 500);
    gtk_window_set_title(GTK_WINDOW(ctx.janela), "Vendas");
    g_signal_connect(ctx.janela, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    ctx.notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(ctx.janela), ctx.notebook);
    
    //acesso
    ctx.acesso = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(ctx.acesso), 10);
    GtkWidget *box_acesso = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    ctx.entrada = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ctx.entrada), "SENHA");
    gtk_entry_set_visibility(GTK_ENTRY(ctx.entrada), FALSE);

    ctx.botao_verificar = gtk_button_new_with_label("VERIFICAR");
    g_signal_connect(ctx.botao_verificar, "clicked", G_CALLBACK(verificar_f), &ctx);
    ctx.botao_visibilidade = gtk_button_new_with_label(" ");
    g_signal_connect(ctx.botao_visibilidade, "clicked", G_CALLBACK(visibilidade), &ctx);

    gtk_grid_attach(GTK_GRID(ctx.acesso), ctx.entrada, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(ctx.acesso), ctx.botao_visibilidade, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(ctx.acesso), ctx.botao_verificar, 0, 1, 2, 1);
    gtk_box_pack_start(GTK_BOX(box_acesso), ctx.acesso, FALSE, TRUE, 0);
    gtk_widget_set_halign(box_acesso, GTK_ALIGN_CENTER);
    //cadastro
    ctx.cad = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(ctx.cad), 10);
    gtk_grid_set_column_spacing(GTK_GRID(ctx.cad), 10);
    ctx.box_cad = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    GtkWidget *cadastro2 = gtk_label_new("CADASTRO");
    ctx.cad_label = gtk_label_new("");
    ctx.cad_nome = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ctx.cad_nome), "NOME DO PRODUTO");
    ctx.cad_valor = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ctx.cad_valor), "VALOR");
    ctx.cad_quantidade = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ctx.cad_quantidade), "ESTOQUE");
    GtkWidget *submeter = gtk_button_new_with_label("ENVIAR");
    g_signal_connect(submeter, "clicked", G_CALLBACK(cadastrar_produto), &ctx);
    GtkWidget *voltar = gtk_button_new_with_label("voltar");
    g_signal_connect(voltar, "clicked", G_CALLBACK(voltar_f), &ctx);
    //organizar cadastro
    gtk_grid_attach(GTK_GRID(ctx.cad), cadastro2, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(ctx.cad), ctx.cad_nome, 0, 1 , 2, 1);
    gtk_grid_attach(GTK_GRID(ctx.cad), ctx.cad_valor, 0, 2 , 1, 1);
    gtk_grid_attach(GTK_GRID(ctx.cad), ctx.cad_quantidade, 1, 2 , 1, 1);
    gtk_grid_attach(GTK_GRID(ctx.cad), submeter, 0, 3 , 2, 1);
    gtk_grid_attach(GTK_GRID(ctx.cad), voltar, 0, 4 , 2, 1);
    gtk_box_pack_start(GTK_BOX(ctx.box_cad), ctx.cad, FALSE, FALSE, 0);
    
    //loja
    GtkWidget *scroll_loja = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_loja), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    
    ctx.lista = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(ctx.lista), 30);
    gtk_grid_set_column_spacing(GTK_GRID(ctx.lista), 15);
    gtk_widget_set_margin_start(ctx.lista, 20);

    GtkWidget *box_lista = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *loja2 = gtk_label_new("LOJA");
    gtk_box_pack_start(GTK_BOX(box_lista), loja2, FALSE, TRUE, 0);
    gtk_widget_set_halign(loja2, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box_lista), ctx.lista, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(scroll_loja), box_lista);
    
    //carrinho
    GtkWidget *scroll_carrinho = gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_carrinho), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    
    ctx.carrinho = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(ctx.carrinho), 30);
    gtk_grid_set_column_spacing(GTK_GRID(ctx.carrinho), 15);
    gtk_widget_set_margin_start(ctx.carrinho, 20);
    
    GtkWidget *box_carrinho = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *carrinho2 = gtk_label_new("CARRINHO");
    gtk_box_pack_start(GTK_BOX(box_carrinho), carrinho2, FALSE, TRUE, 0);
    gtk_widget_set_halign(carrinho2, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box_carrinho), ctx.carrinho, TRUE, TRUE, 0);
    ctx.valor = gtk_label_new("R$ 0.00");
    gtk_box_pack_start(GTK_BOX(box_carrinho), ctx.valor, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(scroll_carrinho), box_carrinho);
    
    //adicionar ao notebook
    GtkWidget *cadastro = gtk_label_new("CADASTRO");
    GtkWidget *carrinho = gtk_label_new("CARRINHO");
    GtkWidget *loja = gtk_label_new("LOJA");
    GtkWidget *acesso = gtk_label_new("ACESSO");
    gtk_notebook_append_page(GTK_NOTEBOOK(ctx.notebook), box_acesso, acesso);
    gtk_notebook_append_page(GTK_NOTEBOOK(ctx.notebook), ctx.box_cad, cadastro);
    gtk_notebook_append_page(GTK_NOTEBOOK(ctx.notebook), scroll_loja, loja);
    gtk_notebook_append_page(GTK_NOTEBOOK(ctx.notebook), scroll_carrinho, carrinho);
    
    gtk_widget_show_all(ctx.janela);
    gtk_widget_hide(ctx.box_cad);
    gtk_main();
}
//quantidade de itens a comprar V
//calculo do total a pagar V
//acesso do adminstrador = aba para selecionar se é cliente ou ADM, aba para receber senha V
//se adicionar produto mais de uma vez ele adiciona na soma ao invés de cadastrar novo V
