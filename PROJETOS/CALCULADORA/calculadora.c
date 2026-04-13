#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

typedef struct{
    GtkWidget *janela;
    GtkWidget *notebook;
    GtkWidget *calc_label;
    GtkWidget *botoes[4][4];
    float numeros[200];
    int operadores[100];
    int valores[4][4];
    float resultado;
    int algarismos;
    int contador_op;
    int contador_num;
    int virgula;
    char txt[200];
}programa;

void previa(programa *ctx){
    char num[32] = "", op[8];
    strcpy(ctx->txt,"");
    for(int c = 0; c <= ctx->contador_num; c++){
        sprintf(num, "%g", (float)ctx->numeros[c]);
        strcat(ctx->txt, num);
        if(c < ctx->contador_num){
            if(ctx->operadores[c] == 3){
                strcpy(op, "+");
            }else if(ctx->operadores[c] == 7){
                strcpy(op, "-");
            }else if(ctx->operadores[c] == 11){
                strcpy(op, "*");
            }else if(ctx->operadores[c] == 15){
                strcpy(op, "/");
            }
            strcat(ctx->txt, op);
        }
    }
    gtk_label_set_text(GTK_LABEL(ctx->calc_label), ctx->txt);
}
void virgula(programa *ctx){
    if(ctx->virgula == 0){
        ctx->virgula = 1;
    }
}
void numeros(programa *ctx, int valor){
    float num;
    if(valor < 3){
        num = valor + 1;
    }else if(valor > 3 && valor < 7){
        num = valor;
    }else if(valor == 13){
        num = 0;
    }else{
        num = valor - 1;
    }
    if(ctx->algarismos == 0){
        ctx->numeros[ctx->contador_num] = 0;
    }
    if(ctx->algarismos == 0 && ctx->virgula == 0){
        ctx->numeros[ctx->contador_num] = num;
    }else if(ctx->algarismos != 0 && ctx->virgula == 0){
        ctx->numeros[ctx->contador_num] = ctx->numeros[ctx->contador_num] * 10 + num;
    }else if(ctx->virgula != 0){
        for(int c = 1; c <= ctx->virgula; c++){
            num = num/10;
        }
        ctx->numeros[ctx->contador_num] = ctx->numeros[ctx->contador_num] + num;
        ctx->virgula++;
    }
    ctx->algarismos++;
    previa(ctx);
}
void operadores(programa *ctx, int valor){
    ctx->operadores[ctx->contador_op] = valor;
    ctx->contador_num++;
    ctx->contador_op++;
    ctx->algarismos = 0;
    ctx->virgula = 0;
    previa(ctx);
}

void igual(programa *ctx){
    printf("igual foi chamado\n");
    ctx->resultado = ctx->numeros[0];
    for(int c = 1; c <= ctx->contador_num; c++){
        if(ctx->operadores[c-1] == 3){
            ctx->resultado = ctx->resultado + ctx->numeros[c];
        }else if(ctx->operadores[c-1] == 7){
            ctx->resultado = ctx->resultado - ctx->numeros[c];
        }else if(ctx->operadores[c-1] == 11){
            ctx->resultado = ctx->resultado * ctx->numeros[c];
        }else if(ctx->operadores[c-1] == 15){
            ctx->resultado = ctx->resultado / ctx->numeros[c];
        }
        ctx->numeros[c] = 0;
        ctx->operadores[c - 1] = 0;     
    }
    char txt[32];
    sprintf(txt, "%g", (float)ctx->resultado);
    gtk_label_set_text(GTK_LABEL(ctx->calc_label), txt);
    ctx->contador_num = 0;
    ctx->contador_op = 0;
    ctx->algarismos = 0;
    ctx->resultado = 0;
    ctx->virgula = 0;
}

static void buttons(GtkWidget *widget, gpointer data){
    programa *ctx = (programa *)data;
    int coluna, linha;
    gtk_container_child_get(GTK_CONTAINER(gtk_widget_get_parent(widget)),widget, "left-attach", &coluna, "top-attach", &linha, NULL);
    int l = linha - 2, c = coluna;
    if((ctx->valores[l][c] < 3)||(ctx->valores[l][c] > 3 && ctx->valores[l][c] < 7) || (ctx->valores[l][c] > 7 && ctx->valores[l][c] < 11) || ctx->valores[l][c] == 13){
        numeros(ctx, ctx->valores[l][c]);
    }else if(ctx->valores[l][c] == 3 || ctx->valores[l][c] == 7 || ctx->valores[l][c] == 11 || ctx->valores[l][c] == 15){
        operadores(ctx, ctx->valores[l][c]);
    }else if(ctx->valores[l][c] == 14){
        igual(ctx);
    }else if(ctx->valores[l][c] == 12){
        virgula(ctx);
    }
}
static void apagar(GtkWidget *widget, gpointer data){
    programa *ctx = (programa *)data;
    int tam = strlen(ctx->txt);
    char provisoria[100];
    if(tam > 0){
        ctx->txt[tam - 1] = '\0';
        if(ctx->numeros[ctx->contador_num] == 0 && ctx->contador_op != 0){
            ctx->contador_num--;
            ctx->contador_op--;
        }else if(ctx->numeros[ctx->contador_num] > 0 && ctx->virgula == 0){
            ctx->algarismos--;
            int num = ctx->numeros[ctx->contador_num];
            ctx->numeros[ctx->contador_num] = (num - (num % 10))/ 10;
        }else if(ctx->virgula > 0){
            sprintf(provisoria, "%g", ctx->numeros[ctx->contador_num]);
            int tam2 = strlen(provisoria);
            provisoria[tam2 - 1] = '\0';
            ctx->numeros[ctx->contador_num] = atof(provisoria);
            ctx->virgula--;
        }

        previa(ctx);
    }
}
static void menu(GtkWidget *widget, gpointer data){

}

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    static programa ctx;
    for(int c = 0; c < 200; c++){
        ctx.numeros[c] = 0;
        if(c < 100){
            ctx.operadores[c] = 0;
        }
    }
    ctx.resultado = 0;
    ctx.algarismos = 0;
    ctx.contador_op = 0;
    ctx.contador_num = 0;
    ctx.virgula = 0;
    strcpy(ctx.txt,"");

    ctx.janela = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(ctx.janela), "CALCULADORA");
    gtk_window_set_default_size(GTK_WINDOW(ctx.janela), 200, 200);
    g_signal_connect(ctx.janela, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    ctx.notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(ctx.janela), ctx.notebook);

    //grid principal
    GtkWidget *grid = gtk_grid_new();
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    ctx.calc_label = gtk_label_new("0");
    gtk_grid_attach(GTK_GRID(grid), ctx.calc_label, 0, 0, 4, 1);
    GtkWidget *apagarB = gtk_button_new_with_label("<-");
    g_signal_connect(apagarB,"clicked", G_CALLBACK(apagar), &ctx);
    GtkWidget *menuB = gtk_button_new_with_label("M");
    g_signal_connect(menuB,"clicked", G_CALLBACK(menu), &ctx);
    gtk_box_pack_start(GTK_BOX(box), menuB, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), apagarB, TRUE, TRUE, 10);
    gtk_grid_attach(GTK_GRID(grid), box, 0, 1, 4, 1);
    int num, val = 0;
    char lab[30];
    for(int l = 0; l < 4; l++){
        for(int c = 0; c < 4; c++){
            ctx.valores[l][c] = val;
            if(val < 3){
                num = val + 1;
                sprintf(lab, "%d", num);
                ctx.botoes[l][c] = gtk_button_new_with_label(lab);
            }else if(val == 3){
                ctx.botoes[l][c] = gtk_button_new_with_label("+");
            }else if(val > 3 && val < 7){
                num = val;
                sprintf(lab, "%d", num);
                ctx.botoes[l][c] = gtk_button_new_with_label(lab);
            }else if(val == 7){
                ctx.botoes[l][c] = gtk_button_new_with_label("-");
            }else if(val > 7 && val < 11){
                num = val - 1;
                sprintf(lab, "%d", num);
                ctx.botoes[l][c] = gtk_button_new_with_label(lab);
            }else if(val == 11){
                ctx.botoes[l][c] = gtk_button_new_with_label("x");
            }else if(val == 12){
                ctx.botoes[l][c] = gtk_button_new_with_label(".");
            }else if(val == 13){
                ctx.botoes[l][c] = gtk_button_new_with_label("0");
            }else if(val == 14){
                ctx.botoes[l][c] = gtk_button_new_with_label("=");
            }else if(val == 15){
                ctx.botoes[l][c] = gtk_button_new_with_label("/");
            }
            val++;
            g_signal_connect(ctx.botoes[l][c], "clicked", G_CALLBACK(buttons), &ctx);
            gtk_grid_attach(GTK_GRID(grid), ctx.botoes[l][c], c, l + 2, 1, 1);
        }
    }
    GtkWidget *labelP = gtk_label_new("calculadora");
    gtk_notebook_append_page(GTK_NOTEBOOK(ctx.notebook), grid, labelP);

    gtk_widget_show_all(ctx.janela);
    gtk_main();
    return 0;
}
//botao de menu