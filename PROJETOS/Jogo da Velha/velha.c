#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
typedef struct{
    GtkWidget *notebook;
    GtkWidget *janela;
    GtkWidget *nome1;
    GtkWidget *nome2;
    GtkWidget *partida;
    GtkWidget *avisos;
    GtkWidget *botoes[3][3];
    GtkWidget *placar;

    const char *n1;
    const char *n2;
    int p1;
    int p2;
    int jogadas;
    int key;
    int ganhador;
    int tabuleiro[3][3];
}contexto;

void popup(contexto *ctx, char *frase){
    GtkWidget *dialogo = gtk_message_dialog_new(GTK_WINDOW(ctx->janela), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", frase);
    gtk_dialog_run(GTK_DIALOG(dialogo));
    gtk_widget_destroy(dialogo);
    
}
void verificar(contexto *ctx){
    for(int c = 0; c < 3; c++){
        if(ctx->tabuleiro[c][0] == ctx->tabuleiro[c][1] && ctx->tabuleiro[c][0] == ctx->tabuleiro[c][2] && ctx->tabuleiro[c][0] != 0){
            if(ctx->tabuleiro[c][0] == 1){
                ctx->ganhador = 1;
            }else{
                ctx->ganhador = 2;
            }
        }
        if(ctx->tabuleiro[0][c] == ctx->tabuleiro[1][c] && ctx->tabuleiro[0][c] == ctx->tabuleiro[2][c] && ctx->tabuleiro[0][c] != 0){
            if(ctx->tabuleiro[0][c] == 1){
                ctx->ganhador = 1;
            }else{
                ctx->ganhador = 2;
            }
        }
    }
    if(ctx->tabuleiro[0][0] == ctx->tabuleiro[1][1] && ctx->tabuleiro[0][0] == ctx->tabuleiro[2][2] && ctx->tabuleiro[0][0] != 0){
        if(ctx->tabuleiro[0][0] == 1){
            ctx->ganhador = 1;
        }else{
            ctx->ganhador = 2;
        }
    }
    if(ctx->tabuleiro[0][2] == ctx->tabuleiro[1][1] && ctx->tabuleiro[1][1] == ctx->tabuleiro[2][0] && ctx->tabuleiro[1][1] != 0){
        if(ctx->tabuleiro[1][1] == 1){
            ctx->ganhador = 1;
        }else{
            ctx->ganhador = 2;
        }
    }
    char frase[100];
    char placar[100];
    if(ctx->ganhador != 0){
        if(ctx->ganhador == 1){
            ctx->p1++;
            sprintf(frase, "%s venceu!", ctx->n1);
            gtk_label_set_text(GTK_LABEL(ctx->avisos), frase);
        }else if(ctx->ganhador == 2){
            ctx->p2++;
            sprintf(frase, "%s venceu!", ctx->n2);
            gtk_label_set_text(GTK_LABEL(ctx->avisos), frase);
        }
        sprintf(placar, "%s = %d   %s = %d", ctx->n1, ctx->p1, ctx->n2, ctx->p2);
        gtk_label_set_text(GTK_LABEL(ctx->placar), placar);
    }
}

static void nomes(GtkWidget *widget, gpointer data){
    contexto *ctx = (contexto *)data;
    ctx->n1 = gtk_entry_get_text(GTK_ENTRY(ctx->nome1));
    ctx->n2 = gtk_entry_get_text(GTK_ENTRY(ctx->nome2));
        char frase[50];
        sprintf(frase, "%s vs %s",  ctx->n1, ctx->n2);
        gtk_label_set_text(GTK_LABEL(ctx->partida), frase);
        char placar[100];
        sprintf(placar, "%s = %d   %s = %d", ctx->n1, ctx->p1, ctx->n2, ctx->p2);
        gtk_label_set_text(GTK_LABEL(ctx->placar), placar);
        gtk_notebook_set_current_page(GTK_NOTEBOOK(ctx->notebook), 1);
        char frase2[100];
        sprintf(frase2, "sejam bem-vindos %s & %s", ctx->n1, ctx->n2);
        popup(ctx, frase2);
}

void avancar(contexto *ctx){
    int i;
    ctx->key = 0;
    ctx->jogadas++;
    char aviso[30];
    if(ctx->jogadas % 2 == 0){
        sprintf(aviso, "vez de %s", ctx->n2);
    }else{
        sprintf(aviso, "vez de %s", ctx->n1);
    }
    gtk_label_set_text(GTK_LABEL(ctx->avisos), aviso);
}

static void botoes(GtkWidget *widget, gpointer data){
    contexto *ctx = (contexto *)data;
    int linha, coluna;
    if(ctx->ganhador == 0 && ctx->key == 0){
        gtk_container_child_get(GTK_CONTAINER(gtk_widget_get_parent(widget)), widget, "left-attach", &coluna, "top-attach", &linha, NULL);
        int L = linha - 2;
        int C = coluna;
        if(ctx->tabuleiro[L][C] == 0){
            char s;
            if(ctx->jogadas % 2 == 0){
                s = 'O';
                ctx->tabuleiro[L][C] = 2;
            }else{
                s = 'X';
                ctx->tabuleiro[L][C] = 1;
            }
            char simbolo[5];
            sprintf(simbolo, "\n%c\n", s);
            gtk_button_set_label(GTK_BUTTON(widget), simbolo);
            ctx->key = 1;
            verificar(ctx);
        }else{
            gtk_label_set_text(GTK_LABEL(ctx->avisos), "posição ocupada");
        }
    }
    if(ctx->ganhador == 0 && ctx->key == 1){
        avancar(ctx);
    }
}

static void resetar(GtkWidget *widget, gpointer data){
    contexto *ctx = (contexto *)data;
    int a, b;
    for(a = 0; a < 3; a++){
        for(b = 0; b < 3; b++){
            ctx->tabuleiro[a][b] = 0;
            gtk_button_set_label(GTK_BUTTON(ctx->botoes[a][b]), "\n   \n");
        }
    }
    gtk_label_set_text(GTK_LABEL(ctx->avisos), "jogo resetado!");
    ctx->ganhador = 0;
    ctx->jogadas = 1;
    ctx->key = 0;
}

static void resetar_p(GtkWidget *widget, gpointer data){
    contexto *ctx = (contexto *)data;
    ctx->p1 = 0;
    ctx->p2 = 0;
    gtk_label_set_text(GTK_LABEL(ctx->avisos), "placar resetado!");
    char placar[100];
    sprintf(placar, "%s = %d   %s = %d", ctx->n1, ctx->p1, ctx->n2, ctx->p2);
    gtk_label_set_text(GTK_LABEL(ctx->placar), placar);
}

static void menu(GtkWidget *widget, gpointer data){
    contexto *ctx = (contexto *)data;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(ctx->notebook), 2);
}

static void jogo(GtkWidget *widget, gpointer data){
    contexto *ctx = (contexto *)data;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(ctx->notebook), 1);
}

static void info(GtkWidget *widget, gpointer data){
    contexto *ctx = (contexto *)data;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(ctx->notebook),3);
}

static void cad(GtkWidget *widget, gpointer data){
    contexto *ctx = (contexto *)data;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(ctx->notebook), 0);
}

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    static contexto ctx;
    ctx.jogadas = 1;
    ctx.ganhador = 0;
    ctx.p1 = 0;
    ctx.p2 = 0;
    
    GtkWidget *grid;
    GtkWidget *grid_B;
    GtkWidget *grid_cad;
    GtkWidget *cadastro_B;
    GtkWidget *grid_menu;
    GtkWidget *menu_B;
    GtkWidget *info_B;
    GtkWidget *box2;
    GtkWidget *submeter;
    GtkWidget *Resetar;
    GtkWidget *ResetarP;
    
    ctx.janela = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(ctx.janela), "jogo da velha 2.0");
    gtk_window_set_default_size(GTK_WINDOW(ctx.janela),200, 300);
    g_signal_connect(ctx.janela, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    ctx.notebook = gtk_notebook_new();
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(ctx.notebook), FALSE);
    gtk_container_add(GTK_CONTAINER(ctx.janela), ctx.notebook);
    
    //Grid_cadastro
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    grid_cad = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid_cad), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid_cad), 10);
    

    ctx.nome1 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ctx.nome1), "jogador 1");
    ctx.nome2 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ctx.nome2), "jogador 2");
    submeter = gtk_button_new_with_label("Submeter");
    g_signal_connect(submeter, "clicked", G_CALLBACK(nomes), &ctx);
    GtkWidget *avancar = gtk_label_new("\n\n\n\n\nDIGITE O NOME DOS JOGADORES");
    
    gtk_grid_attach(GTK_GRID(grid_cad), avancar, 0, 0, 3, 1);
    gtk_box_pack_start(GTK_BOX(box1), ctx.nome1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), ctx.nome2, TRUE, TRUE, 0);
    gtk_grid_attach(GTK_GRID(grid_cad), box1, 0, 1, 3, 1);
    gtk_grid_attach(GTK_GRID(grid_cad), submeter, 0, 2, 3, 1);
    
    GtkWidget *cadastro = gtk_label_new("CADASTRO");
    gtk_notebook_append_page(GTK_NOTEBOOK(ctx.notebook), grid_cad, cadastro);

    //Grid Principal
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    
    
    ctx.partida = gtk_label_new("...");
    ctx.avisos = gtk_label_new("BEM VINDO!");
    ctx.placar = gtk_label_new("jogador 1 = 0   jogador 2 = 0");
    
    gtk_grid_attach(GTK_GRID(grid), ctx.partida, 0, 0, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), ctx.avisos, 0, 1, 3, 1);
    
    for(int c = 0; c < 3; c++){
        for(int x = 0; x < 3; x++){
            ctx.botoes[c][x] = gtk_button_new_with_label("\n   \n");
            ctx.tabuleiro[c][x] = 0;
            g_signal_connect(ctx.botoes[c][x], "clicked", G_CALLBACK(botoes), &ctx);
            gtk_grid_attach(GTK_GRID(grid), ctx.botoes[c][x], x, c + 2, 1, 1);
        }
    }
    
    box2 =gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    Resetar = gtk_button_new_with_label("Reset Game");
    g_signal_connect(Resetar, "clicked", G_CALLBACK(resetar), &ctx);
    gtk_box_pack_start(GTK_BOX(box2), Resetar, TRUE,TRUE, 0);
    ResetarP = gtk_button_new_with_label("Reset Score");
    g_signal_connect(ResetarP, "clicked", G_CALLBACK(resetar_p), &ctx);
    gtk_box_pack_start(GTK_BOX(box2), ResetarP, TRUE,TRUE, 0);
    menu_B = gtk_button_new_with_label("MENU");
    g_signal_connect(menu_B, "clicked", G_CALLBACK(menu), &ctx);
    gtk_box_pack_start(GTK_BOX(box2),menu_B, TRUE, TRUE, 0);
    gtk_grid_attach(GTK_GRID(grid), box2, 0, 6, 3, 1);

    gtk_grid_attach(GTK_GRID(grid), ctx.placar, 0, 5, 3, 1);

    GtkWidget *game = gtk_label_new("JOGO");
    gtk_notebook_append_page(GTK_NOTEBOOK(ctx.notebook), grid, game);

    //grid menu
    GtkWidget *box3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    cadastro_B = gtk_button_new_with_label("CADASTRO");
    g_signal_connect(cadastro_B, "clicked", G_CALLBACK(cad), &ctx);
    grid_B = gtk_button_new_with_label("JOGO");
    g_signal_connect(grid_B, "clicked", G_CALLBACK(jogo), &ctx);
    info_B = gtk_button_new_with_label("INFORMAÇÔES");
    g_signal_connect(info_B, "clicked", G_CALLBACK(info), &ctx);
    GtkWidget *menu_label = gtk_label_new("\n\nMENU DE SELEÇÔES");

    gtk_box_pack_start(GTK_BOX(box3), menu_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), cadastro_B, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), grid_B, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), info_B, TRUE, TRUE, 0);

    GtkWidget *menu_top = gtk_label_new("MENU");
    gtk_notebook_append_page(GTK_NOTEBOOK(ctx.notebook), box3, menu_top);
    
    //grid info
    GtkWidget *box4 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *infob = gtk_button_new_with_label("retornar");
    g_signal_connect(infob, "clicked", G_CALLBACK(menu), &ctx);
    GtkWidget *info_label = gtk_label_new("Programa feito por Rian Leal");
    gtk_box_pack_start(GTK_BOX(box4), info_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box4), infob, TRUE, TRUE, 0);
    GtkWidget *info_top = gtk_label_new("INFORMAÇÔES");

    gtk_notebook_append_page(GTK_NOTEBOOK(ctx.notebook), box4, info_top);
    
    gtk_widget_show_all(ctx.janela);
    gtk_main();
}