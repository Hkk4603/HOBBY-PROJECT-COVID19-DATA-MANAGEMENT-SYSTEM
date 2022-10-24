#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

int cases = 0, deaths = 0, discharges = 0;
int count_key_tries = 0;
int Aadhar_flag = 0, check = 0;
char keys[] = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"};

// ************************************************DOCTOR LOGIN**********************************************************************
void doctor_login(void);
void display_doc_menu(char *, char *, char *);
void view_patient();
void redirecting(char *);
void confirm_pat_vac(char *);
void edit_doselog(char *, char *);
int check_sltbooking(char *);
void update();
void display_vac_status();
void display_patient(char *);

//****************************************************ACCOUNT CREATION*********************************************************
void create_doc_account();
void patient_create_acc();
void create_account();

//****************************************************VIEW COVID ANALYTICS***************************************************************
void plot();

//*************************************Prototype Of Patient Login Modules*******************************************
void patient_login();
void patient_menue(char *ph_no, char *patient_aadhar, char *dummy_name);
void display_vac_status(char *aadhar);
void compare(char *aadhar);
void hosp_book(char *aadhar, char *hosp_name, char *pin_code_para);
void pin(char *aadhar, char *pin_code);
void vac_slot_booking();
int checks(char *s);
void sec_key_gen(FILE *fp);
void registration(char *ph_no);

//*************************************************HELP AND FEEDBACK********************************************************
void help();
void feedback();
void help_feedback();
//*************************************************MAIN FUNCTION*******************************************************************
int main()
{
    int ch;
    while (1)
    {
        system("cls");
        printf("WELCOME TO COVID-19 MANAGEMENT SYSTEM\n\n");
        printf("MENU:\n1. CREATE ACCOUNT\n2. DOCTOR LOGIN\n3. PATIENT LOGIN\n4. VIEW COVID ANALYTICS\n5. HELP AND FEEDBACK\n6. EXIT\n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            create_account();
            break;
        case 2:
            doctor_login();
            break;
        case 3:
            patient_login();
            break;
            break;
        case 4:
            plot();
            break;
        case 5:
            help_feedback();
            break;
        case 6:
            exit(0);
        default:
            printf("******Please select a valid option******");
            continue;
        }
    }
    // Don't Disturb the code below
    FILE *fp = fopen("update_cases.txt", "a");
    fprintf(fp, "\n%d %d %d", cases, deaths, discharges);
    fclose(fp);
    // plot();
    return 0;
}

// ****************************************************DOCTOR LOGIN****************************************************************

void display_doc_menu(char doc_id[], char hosp_name[], char name_comp[])
{
    system("cls");
    int flag, count = 1, count1 = 1, choice;
    // char doc_id_comp[20], password[20], doc_name[20], hosp_name[30];
    doc_id[strlen(doc_id)] = '\n';
    FILE *fp = 0;
    fp = fopen("doc_acc.txt", "r");
    if (fp == NULL)
    {
        printf("System Error!!\nTRY AGAIN\n");
        return;
    }

    while (1)
    {
        printf("\t\t\tHospital Name: %s\n", hosp_name);
        printf("\t\t\tWELCOME %s \n\t\t\t ID: %s\n", name_comp, doc_id);
    label_menu:
        printf("\t\t\tDOCTOR MENU\n1.Update Details\n2.Confirm Patient Vaccination\n3.View Patient Details\n4.Logout\n");
        printf("Enter Your Choice\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            redirecting("in");
            update();
            break;
        case 2:
            redirecting("in");
            confirm_pat_vac(hosp_name);
            break;
        case 3:
            redirecting("in");
            view_patient();
            break;
        case 4:
            printf("Succesfully Logged Out\n");
            redirecting("to main menu");
            return;
        default:
            printf("Enter A Valid Choice\n");
            goto label_menu;
        }
    }
}

//******This module is not working properly for password trials( it only verifies the first password trial )
void doctor_login()
{
    FILE *fp = 0;
    int count, count1, flag = 0, trial = 3, i = 0;
    char hosp_comp[20], name_comp[20];
    fp = fopen("doc_acc.txt", "r");
    if (fp == NULL)
    {
        printf("System Error!!\nTRY AGAIN\n");
        return;
    }
    char doc_id[20], password[20], doc_id_comp[20], password_comp[20];
label_login:
    system("cls");
    printf("Enter Doctor ID:\n");
    scanf("%s", doc_id);
    if ((strlen(doc_id) != 9) && (strncmp(doc_id, "DID", 3) != 0))
    {
        printf("Invalid Format\n");
        redirecting("in");
        goto label_login;
    }
    rewind(fp);
    count = 1;
    count1 = 1;
    while (!feof(fp))
    {
        fscanf(fp, "%s", doc_id_comp);
        if (strcmp(doc_id, doc_id_comp) == 0)
        {
            goto label_pass;
        }
        if (count == count1)
            count1 += 4;
        count++;
    }
    printf("Account Not Found\nTry Again\n");
    redirecting("in");
    goto label_login;
label_pass:
    while (trial != 0)
    {
        i = 0;
        printf("Enter The Password: \n");
        do
        {
            password[i] = getch();
            if (password[0] == 8 || password[0] == 13)
            {
                continue;
            }
            if (password[i] != 8)
            {
                printf("*");
            }
            if (password[i] == 8)
            {
                printf("\b \b");
                i--;
                continue;
            }
            i++;
        } while (password[i - 1] != 13);
        password[i - 1] = '\0';

        fscanf(fp, "%s", password_comp);
        if (strcmp(password, password_comp) != 0)
        {
            printf("Wrong Password\nTry Again\n");
            fseek(fp, -strlen(password_comp), SEEK_CUR);
            // sleep(3);
            trial--;
            printf("You Have %d Attempts Remaining\n\n", trial);
            goto label_pass;
        }
        else
        {
            printf("\n\nLogin Successful\n\n");
            redirecting("to menu");
            fscanf(fp, "%s", name_comp);
            fscanf(fp, "%s", hosp_comp);
            display_doc_menu(doc_id, hosp_comp, name_comp);
            flag = 1;
            break;
        }
    }
    if (flag != 1)
    {
        printf("Too Many Login Attempts, Try Again Later\n");
        redirecting("in");
    }
    return;
}

void view_patient()
{
    int flag = 0, count = 1, count1 = 1;
    char ch;
    char patient_num[20], patient_num_comp[20], patient_name[20], patient_pass[20], patient_aadhar[20];
    FILE *fp = 0;
    fp = fopen("patient_acc.txt", "r");
    if (fp == NULL)
    {
        printf("System Error!!\nTRY AGAIN\n");
        return;
    }
label_phno:
    system("cls");
    printf("VIEW PATIENT DETAILS\n");
    printf("Enter Patient Phone Number\n");
    scanf("%s", patient_num);
    patient_num[strlen(patient_num)] = '\n';
    rewind(fp);
    while (!feof(fp))
    {
        fgets(patient_num_comp, sizeof(patient_num_comp), fp);

        if ((strncmp(patient_num, patient_num_comp, strlen(patient_num_comp)) == 0) && (count == count1))
        {
            flag = 1;
            goto label_patient;
        }
        if (count == count1)
            count1 += 4;
        count++;
    }
    if (flag != 1)
    {
        printf("Patient Not Found\nTry Again\n");
        redirecting("in");
        goto label_phno;
    }

label_patient:
    fgets(patient_aadhar, sizeof(patient_aadhar), fp);
    fgets(patient_name, sizeof(patient_name), fp);
    fgets(patient_pass, sizeof(patient_pass), fp);
    // fgets(vacc_status, sizeof(vacc_status), fp); //After Updating Module
    system("cls");
    printf("Name: %s", patient_name);
    printf("PhNo: %s", patient_num_comp);
    printf("Aadhar Number: %s", patient_aadhar);
    redirecting(" ");
    display_patient(patient_num);
    printf("\nPlease press ENTER to continue\n\n");
    getch();
    redirecting("to menu");
    return;
}

void confirm_pat_vac(char *hospital)
{
    system("cls");
    int i = 0, idx = 0;
    char vaccine[10];
    long int offset;
    FILE *fp;
    fp = fopen("reg_log.txt", "r");
    char ph_no[11], sec_key[8], ph_no_cmp[11], sec_key_cmp[8], aadhar_no[13], aadhar_cmp[13];
enter_ph_no:
    printf("\n");
    printf("Enter the phone number of the patient: ");
    scanf("%s", ph_no);
    if (strlen(ph_no) != 10 && ph_no[0] != 0) // modified
    {
        printf("\nEntered number is invalid. Phone number must have 10 digits\n\n");
        redirecting("in");
        goto enter_ph_no;
    }
    while (!feof(fp))
    {
        fscanf(fp, "%s", ph_no_cmp);
        if (!strcmp(ph_no, ph_no_cmp))
        {
        aadhar_entry:
            printf("\nEnter the AADHAR NUMBER: ");
            scanf("%s", aadhar_no);
            if (strlen(aadhar_no) != 12)
            {
                printf("\n\nAADHAR NUMBER must contain 12 digits\n\n");
                goto aadhar_entry;
            }
        aadhar_check:
            fscanf(fp, "%s", aadhar_cmp);
            if (strlen(aadhar_cmp) == 10 || feof(fp))
            {
                printf("\n\n***The entered AADHAR NUMBER is not registered***\n\n");
                return;
            }

            if (!strcmp(aadhar_no, aadhar_cmp))
            {
                fscanf(fp, "%s", sec_key_cmp);
            sec_key_entry:
                if (count_key_tries < 3)
                {
                    printf("\nEnter the SECRET KEY: ");
                    do
                    {
                        sec_key[idx] = getch();
                        if (sec_key[0] == 8 || sec_key[0] == 13)
                        {
                            continue;
                        }
                        if (sec_key[idx] != 8)
                        {
                            printf("*");
                        }
                        if (sec_key[idx] == 8)
                        {
                            printf("\b \b");
                            idx--;
                            continue;
                        }
                        idx++;
                    } while (sec_key[idx - 1] != 13);
                    sec_key[idx - 1] = '\0';
                    idx = 0;
                    count_key_tries++;

                    if (!strcmp(sec_key, sec_key_cmp))
                    {
                        printf("\n\n");
                        printf("***SUCCESSFULLY VERIFIED***\n\n");
                        if (!check_sltbooking(aadhar_no))
                        {
                            printf("****SLOT NOT BOOKED****\n\n");
                            redirecting("in");
                            return;
                        }
                        else
                        {
                            edit_doselog(aadhar_no, hospital);
                        }
                        count_key_tries = 0;
                        // should add the vaccination details to be updated in the vaccination status_file.txt in patient login
                        fclose(fp);
                        redirecting("to main menu in");
                        return;
                    }
                    else
                    {
                        if (count_key_tries < 3)
                        {
                            printf("\n\n***INCORRECT PASSWORD***\n\n");
                            printf("NUMBER OF TRIES LEFT %d\n\n", 3 - count_key_tries);
                            printf("Please try again\n\n");
                        }
                        goto sec_key_entry;
                    }
                }
                else
                {
                    printf("\n****Number of tries EXCEEDED****\n\n");
                    count_key_tries = 0;
                    return;
                }
            }
            else
            {
                goto aadhar_check;
            }
        }
    }

    printf("\n***Patient details not found***\n");
    fclose(fp);
    return;
}

void edit_doselog(char *aadhar_no, char *hospital)
{
    char aadh_cmp[13], vac_name[12];
    int i, check = 0, dose, ch;
    FILE *fp = fopen("dose_log.txt", "r+");
    while (!feof(fp))
    {
        fscanf(fp, "%s", aadh_cmp);
        if (strlen(aadh_cmp) == 12 && !strcmp(aadh_cmp, aadhar_no))
        {
            fscanf(fp, "%d", &dose);
            fscanf(fp, "%d", &dose);
            if (dose == 0)
            {
                fseek(fp, -1, SEEK_CUR);
                fprintf(fp, "1");
                printf("***SECOND DOSE COMPLETED***\n\n");
                fclose(fp);
                return;
            }
            check = 1;
        }
    }

    if (check == 0)
    {
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "%s\n", aadhar_no);
        fprintf(fp, "1\n");
        fprintf(fp, "0\n");
        printf("Select the vaccine name choose by Patient\n\n");
        printf("1. COVAXIN\n2. COVISHIELD\n");
    ch_entry:
        printf("Enter the choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            strcpy(vac_name, "COVAXIN");
            break;
        case 2:
            strcpy(vac_name, "COVISHIELD");
            break;
        default:
            printf("\nPlease make a valid choice\n\n");
            goto ch_entry;
        }
        fprintf(fp, "%s\n", vac_name);
        printf("\n\n***FIRST DOSE COMPLETED***\n\n");
        fprintf(fp, "%s\n", hospital);
        fclose(fp);
        return;
    }
    printf("PATIENT IS COMPLETELY VACCINATED\n\n");
    fclose(fp);
    return;
}

int check_sltbooking(char *aadh_no)
{
    FILE *fp = fopen("vaccination_book.txt", "r");
    char aadh_cmp[13];
    while (!feof(fp))
    {
        fscanf(fp, "%s", aadh_cmp);
        if (strlen(aadh_cmp) == 12 && !strcmp(aadh_no, aadh_cmp))
            return 1;
    }
    return 0;
}

void update()
{
    FILE *fp = 0;
    fp = fopen("update_cases.txt", "a+");
    int no_of_cases, no_of_deaths, no_of_discharges;
    printf("\n");
    printf("Enter the number of cases: ");
    scanf("%d", &no_of_cases);
    printf("\n");
    printf("Enter the number of deaths: ");
    scanf("%d", &no_of_deaths);
    printf("\n");
    printf("Enter the number of discharges: ");
    scanf("%d", &no_of_discharges);
    printf("\n");
    cases += no_of_cases;
    deaths += no_of_deaths;
    discharges += no_of_discharges;
    printf("\n...UPDATING DATA...\n\n");
    sleep(2);
    system("cls");
    printf("***DATA UPDATED SUCCESSFULLY***\n\n");
    sleep(2);
    system("cls");
    fclose(fp);
}

void redirecting(char *msg)
{
    int i = 3;
    printf("redirecting %s ...  ", msg);
    while (i > 0)
    {
        printf("\b \b%d", i);
        sleep(1);
        i--;
    }
    system("cls");
    printf("\n");
}

//****************************************************ACCOUNT CREATION*********************************************************
void create_account()
{
    system("cls");
    while (1)
    {
        system("cls");
        int ch;
        printf("1.Create Doctor Account\n\n2.Create Patient Account\n\n3.return to menue\n\n");
        printf("Enter Your Choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            create_doc_account();
            break;
        case 2:
            patient_create_acc();
            break;
        case 3:
            return;
        }
    }
}

void create_doc_account()
{
    system("cls");
    int flag = 0, count = 1, temp = 1, counter = 5, Id_proof_count = 4;
    char Doc_Id[10], Doc_Password[9], doc_confirm_password[9],
        Doc_Id_compare[10], doc_proof[15], doc_proof_compare[15],
        hospital_name[50], Doc_Name[50];
    static int len = 9;
    static char Id_gen[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static char ini[4] = "DID";
    FILE *fp = NULL, *fp1 = NULL;
    fp = fopen("doc_acc.txt", "a+");   // file pointer pointing to doc_account
    fp1 = fopen("doc_proof.txt", "r"); // file pointer pointing to doc_proof
    if (fp == NULL || fp1 == NULL)
    {
        printf("FILE DOESNOT EXIST\n");
        return;
    }
ID_Generation:
    srand(time(NULL));
    for (int i = 0; i < 6; i++)
    {
        Id_gen[i] = Id_gen[rand() % (sizeof(Id_gen) - 1)];
    }
    strcat(ini, Id_gen);
    ini[9] = 0;
    while (!feof(fp))
    {
        fscanf(fp, " %[^\n]s", Doc_Id_compare);
        if ((strcmp(Doc_Id_compare, Doc_Id) == 0) && count == temp)
        {
            goto ID_Generation;
        }
        if (count == temp)
            temp = temp + 4;
        count++;
    }
    count = 1;
    temp = 1;
    printf("Enter The Following Details To Create Account:\n\n");
lable1:
    printf("Enter Doctor ID(format is DID******,random suggestion: %s): ", ini);
    scanf("%s", Doc_Id);
    rewind(fp); // sets the file pointer to begining of file,helps in iterating from begining of file
    count = 1;
    temp = 1;
    if (strlen(Doc_Id) < 9 || strlen(Doc_Id) > 9)
    {
        printf("\nDoctor ID Should Be Only 9 Charactes,Try again\n\n");
        goto lable1;
    }
    if (strncmp(Doc_Id, "DID", 3) != 0)
    {
        printf("Doctor Id Should Be Of Format DID******,Try Again\n");
        goto lable1;
    }
    while (!feof(fp))
    {
        fscanf(fp, " %[^\n]s", Doc_Id_compare);
        if ((strcmp(Doc_Id_compare, Doc_Id) == 0) && count == temp) // count helps to only compare id's not passwords
        {
            printf("\n\nThis Doctor ID Already Exist,Try Another ID\n");
            sleep(2);
            while (counter != 0)
            {
                system("cls");
                printf("Redirecting...in...%d\n", counter);
                sleep(1);
                counter--;
            }
            counter = 5;
            system("cls");
            goto lable1;
        }
        if (count == temp)
            temp = temp + 4;
        count++;
    }
    fprintf(fp, "%s\n", Doc_Id);
    printf("\n");
    printf("Enter New Password:");
    scanf("%s", Doc_Password);
    printf("\n");
lable2:
    printf("Confirm Password: ");
    scanf("%s", doc_confirm_password);
    if (strcmp(Doc_Password, doc_confirm_password))
    {
        printf("Password Doesn't Match,Try Again\n");
        goto lable2;
    }
    fprintf(fp, "%s\n", Doc_Password);
    printf("\n");
    printf("Enter The Doctor Name: ");
    scanf(" %[^\n]s", Doc_Name);
    fprintf(fp, "%s\n", Doc_Name);
    printf("\n");
    printf("Enter Hospital Name: ");
    scanf(" %[^\n]s", hospital_name);
    fputs(hospital_name, fp);
    fprintf(fp, "\n");
lable3:
    printf("\nEnter Doctor Proof(Aadhar number): ");
    scanf("%s", doc_proof);
    rewind(fp1);
    while (!feof(fp1))
    {
        fscanf(fp1, "%s", doc_proof_compare);
        if (strcmp(doc_proof, doc_proof_compare) == 0)
        {
            flag = 1;
            while (counter != 0)
            {
                system("cls");
                printf("*********Account Successfully Created**********\n\n");
                printf("Redirecting...in...%d\n", counter);
                sleep(1);
                counter--;
            }
            system("cls");
            break;
        }
    }
    if (Id_proof_count == 0)
    {
        printf("*****TOO MANY ATTEMPTS, TRY LATER*****\n");
        return;
    }
    if (flag != 1 && Id_proof_count > 0)
    {
        printf("Doctor Proof Not Found,%d Chances Left\n", Id_proof_count);
        Id_proof_count--;
        goto lable3;
    }
    fclose(fp);
    fclose(fp1);
    return;
}

void patient_create_acc()
{
    char name[50], number_inp[11], fnum[11], password[15], patient_aadhar[14];
    int count = 1, temp = 1, counter = 5;
    FILE *fptr;
    fptr = fopen("patient_acc.txt", "a+");
ph_no:
    printf("\nenter your phone number: ");
    scanf(" %[^\n]s", number_inp);
    char num_in[11];
    strcpy(num_in, number_inp);
    // printf("\n\n");
    if (strlen(num_in) < 10)
    { // change
        printf("INVALID NUMBER\n");
        goto ph_no;
    }

    rewind(fptr);
    while (!feof(fptr))
    {
        fscanf(fptr, " %[^\n]s", fnum);
        if ((strcmp(num_in, fnum) == 0) && temp == count)
        {
            printf("\n\nthe account already exists,try again\n\n");
            goto ph_no;
        }
        if (temp == count)
            temp = temp + 4;
        count++;
    }
    fprintf(fptr, "%s\n", number_inp);
aadhar:
    printf("enter your aadhar number: ");
    scanf("%s", patient_aadhar);
    if (patient_aadhar[0] == '0' || strlen(patient_aadhar) != 12)
    {
        printf("Invalid aadhar number,try again\n\n");
        goto aadhar;
    }
    fprintf(fptr, "%s\n", patient_aadhar);
    printf("\n\n");
    printf("enter your name: ");
    scanf(" %[^\n]s", name);
    fprintf(fptr, "%s\n", name);
    printf("\n\n");
    printf("Enter password: ");
    scanf("%s", password);
    fprintf(fptr, "%s\n", password);
    printf("\n\n");
    while (counter != 0)
    {
        system("CLS");
        printf("****Account Successfully Created***\n\n");
        printf("Redirecting...in...%d\n", counter);
        sleep(1);
        counter--;
    }
    fclose(fptr);
}

//****************************************************VIEW COVID ANALYTICS***************************************************************

void plot()
{
    int i;
    FILE *gnupipe = NULL;
    char *Gnucommands[] = {"set title \"CASES(purple)| DEATHS(green) | DISCHARGES(light blue)\"", "set style data histogram", "set style fill transparent solid 0.5 noborder", "set palette model RGB defined (0 \"green\", 1 \"yellow\", 2 \"red\")", "set xlabel \"DAYS\"", "set ylabel \"cases,deaths and discharges\"", "plot for [COL=1:3:1] 'update_cases.txt' using COL ,' ' using 1:2:($2<=10 ? 0 : $2<=20 ? 1 : 2) with boxes palette"};

    gnupipe = _popen("gnuplot -persitent", "w");

    for (i = 0; i < 7; i++)
    {
        fprintf(gnupipe, "%s\n", Gnucommands[i]);
    }

    return;
}

//*******************************************************PATIENT LOGIN**********************************************************************

void patient_login()
{
    system("CLS");
    printf("*********Patient Login**********\n\n");
    char ph_no[15], ph_no_compare[15], patient_password[15], patient_password_compare[15], dummy_name[50], patient_aadhar[15];
    int count = 1, temp = 1, password_attempt = 3, choice;
    FILE *fp;
    fp = fopen("patient_acc.txt", "a+");
    if (fp == NULL)
    {
        printf("System Error!!\nTRY AGAIN\n");
        return;
    }
ph_input:
    printf("Enter Patient phone number: ");
    scanf("%s", ph_no);
    if (strlen(ph_no) != 10 || ph_no[0] == 0)
    {
        printf("\n\nInvalid phone number,Try Again");
        redirecting("in");
        goto ph_input;
    }
    rewind(fp);
    while (!feof(fp))
    {
        fscanf(fp, "%s", ph_no_compare);
        if (strcmp(ph_no_compare, ph_no) == 0 && temp == count)
        {
            fscanf(fp, "%s", patient_aadhar);
            fscanf(fp, " %[^\n]s", dummy_name);
            goto pass_input;
        }
        if (count == temp)
            temp += 4;
        count++;
    }
    printf("Number is not Registered\n\n");
pass_input:
    printf("\n\nEnter your password: ");
    scanf("%s", patient_password);
    fscanf(fp, "%s", patient_password_compare);
    if (strcmp(patient_password, patient_password_compare) != 0)
    {
        password_attempt--;
        if (password_attempt == 0)
        {
            printf("\n*****Alert Too Many Attempts,Try Later*****\n");
            return;
        }
        printf("\nInvalid password,Try Again,%d attempts remaining\n\n", password_attempt);
        fseek(fp, -strlen(patient_password_compare), SEEK_CUR);
        goto pass_input;
    }
    printf("\n\n*****Login Successfull*****\n\n");
    sleep(2);
    system("CLS");
    patient_menue(ph_no, patient_aadhar, dummy_name);
}

void patient_menue(char *ph_no, char *patient_aadhar, char *dummy_name)
{
    int ch;
    printf("\t\t\t\t\t\tPatient Phone Number: %s\n", ph_no);
    printf("\t\t\t\t\t\tPatient Aadhar: %s\n", patient_aadhar);
    printf("\t\t\t\t\t\tPatient Name: %s\n", dummy_name);
    printf("1.Registration\n\n2.Book Vaccination Slot\n\n3.Check Vaccination Status\n\n4.Logout\n\n");
    printf("Enter Your Choice: ");
    scanf("%d", &ch);
    switch (ch)
    {
    case 1:
        registration(ph_no);
        break;
    case 2:
        vac_slot_booking();
        break;
    case 3:
        display_vac_status(patient_aadhar);
        break;
    case 4:
        return;
    }
}

void display_patient(char *patient_num)
{
    int flag = 0, count = 1, count1 = 1;
    char patient_num_comp[20], patient_name[20], patient_aadhar[20], patient_sec_key[10], temp[20], reg_compare[20], patient_pass[20], choice[20];
    FILE *fp1 = 0, *fp2 = 0;
label_1:
    fp1 = fopen("patient_acc.txt", "r");
    fp2 = fopen("reg_log.txt", "r");
    if (fp1 == NULL || fp2 == NULL)
    {
        printf("System Error!!\nTRY AGAIN\n");
        return;
    }
    while (!feof(fp1))
    {
        // fscanf(fp, "%[^\n]s", patient_num_comp);
        fgets(patient_num_comp, sizeof(patient_num_comp), fp1);
        if ((strncmp(patient_num_comp, patient_num, strlen(patient_num)) == 0) && count == count1)
        {
            goto label_patient;
        }
        if (count == count1)
            count1 += 4;
        count++;
    }
label_patient:
    fgets(patient_aadhar, sizeof(patient_aadhar), fp1);
    fgets(patient_name, sizeof(patient_name), fp1);
    fgets(patient_pass, sizeof(patient_pass), fp1);
    system("CLS");
    printf("Name: %s", patient_name);
    printf("PhNo: %s", patient_num_comp);
    rewind(fp2);
    while (!feof(fp2))
    {
        fgets(reg_compare, sizeof(reg_compare), fp2);
        if ((strncmp(patient_num, reg_compare, strlen(patient_num)) == 0) && ((strlen(reg_compare) == 11) || (strlen(reg_compare) == 12)))
        {
            flag = 1;
            goto label_aadhar;
        }
    }
    if (flag == 0)
    {
        printf("You Have Not Yet Registered For Vaccination\n");
        getch();
        return;
    }
label_aadhar:
    count = 1;
    do
    {
        // fgets(patient_name, sizeof(patient_name), fp2);
        //  if ((((strlen(patient_name) == 11) || (strlen(patient_name) == 12)) && (isdigit(patient_name[1]))) || (feof(fp2)))
        //  {
        //      break;
        //  }
        //  printf("Case %d: \n", count++);
        // printf("%s", patient_name);

        fgets(patient_aadhar, sizeof(patient_aadhar), fp2);
        printf("%s", patient_aadhar);
        fgets(temp, sizeof(temp), fp2);
        // fseek(fp2, strlen(temp), SEEK_CUR);

    } while (!feof(fp2));
    printf("Enter The Aadhar Number: \n");
    scanf("%s", choice);
    rewind(fp2);
    while (!feof(fp2))
    {
        fgets(temp, sizeof(temp), fp2);
        if (strncmp(patient_num, temp, sizeof(patient_num)) == 0)
        {
            break;
        }
    }
    flag = 0;
    while (!feof(fp2))
    {
        fgets(temp, sizeof(temp), fp2);
        if ((((strlen(temp) == 11) || (strlen(temp) == 12)) && (isdigit(temp[1]))) || (feof(fp2)))
        {
            break;
        }
        if (strncmp(temp, choice, 12) == 0)
        {
            flag = 1;
            goto label_status;
        }
    }
    if (flag != 1)
    {
        printf("Please Enter A Valid Aadhar Number\n");
        // redirecting();
        goto label_1;
    }
label_status:
    display_vac_status(temp);
    return;
}

void display_vac_status(char *aadhar)
{
    // printf("BreakPOint1\n");
    char compare[20], hosp[50], vaccine[20], temp[20], pin[10];
    int dose1, dose2, flag = 0;
    FILE *fp1 = 0, *fp2 = 0;
    fp1 = fopen("pat_conf.txt", "r");
    fp2 = fopen("vac_book.txt", "r");
    if (fp1 == NULL || fp2 == NULL)
    {
        printf("System Error!!\nTRY AGAIN\n");
        return;
    }
    while (!feof(fp1))
    {
        fgets(compare, sizeof(compare), fp1);
        if (strncmp(compare, aadhar, sizeof(aadhar)) == 0)
        {
            goto label_view;
            break;
        }
    }
    // printf("breakPOint2\n");
    printf("Vaccination Status: Unvaccinated (0 Doses)\n");
    while (!feof(fp2))
    {
        fgets(temp, sizeof(temp), fp2);
        if (strncmp(temp, aadhar, strlen(aadhar)) == 0)
        {
            fgets(hosp, sizeof(hosp), fp2);
            fgets(pin, sizeof(pin), fp2);
            printf("Vaccination Slot Booked\n");
            printf("%s", hosp);
            printf("%s", pin);
            flag = 1;
            goto label_skip;
            break;
        }
    }
    if (flag != 1)
    {
        printf("Vaccination Slot Not Booked\n");
        goto label_skip;
    }
// print if booked slot
label_view:
    fscanf(fp1, "%d", &dose1);
    fscanf(fp1, "%d\n", &dose2);
    fgets(vaccine, sizeof(vaccine), fp1);
    fgets(hosp, sizeof(hosp), fp1);
    printf("Aadhar Number: %s", aadhar);
    if (dose1 == 1)
    {
        printf("Vaccine Name: %s", vaccine);
        printf("Vaccination Status: ");
        if (dose2 == 0)
        {
            printf("Partially Vaccinated (1 Dose)\n");
            // print if booked for 2nd dose
        }
        if (dose2 == 1)
        {
            printf("Fully Vaccinated (2 Doses)\n");
        }
        printf("Vaccination Center: %s", hosp);
    }
label_skip:
    // printf("Success\n");
    return;
}

void compare(char *aadhar)
{
    FILE *fp;
    char aadhar_compare[13];
    fp = fopen("reg_log.txt", "r+");
    if (strlen(aadhar) != 12 || aadhar[0] == '0' || aadhar[0] == '1')
    {
        printf("Invalid Aadhar,try again\n\n");
        Aadhar_flag = 1;
        redirecting("in");
        return;
    }
    while (!feof(fp))
    {
        fscanf(fp, "%s", aadhar_compare);
        if (!strcmp(aadhar, aadhar_compare))
        {
            printf("\n\nAadhar Found,Proceeding to booking\n\n");
            sleep(1);
            redirecting("in");
            return;
        }
    }
    printf("\n\nAadhar Not Registered,Try Again After Registration\n\n");
    sleep(2);
    Aadhar_flag = 1;
}

void hosp_book(char *aadhar, char *hosp_name, char *pin_code_para)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE *fp1;
    fp1 = fopen("vac_book.txt", "a+");
    char decision;
    printf("\t\t\t\t\t\tDo You Want To Finalize The Hospital-%s\n\n", hosp_name);
    // strcat(hosp_name,"-");
    // strcat(hosp_name,pin_code_para);
    printf("Enter y if You Want To Proceed,n if u want to go back to menue: ");
    scanf("\n%c", &decision);
    printf("\n\n");
    if (decision == 'n')
    {
        sleep(1);
        check = 1;
        return;
    }
    printf("Booking Slot.....Please Wait\n\n");
    fprintf(fp1, "%s\n", aadhar);
    fprintf(fp1, "%s\n", hosp_name);
    fprintf(fp1, "%s\n", pin_code_para);
    // fprintf(fp1,"-%s",pin_code_para);
    fprintf(fp1, "%d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    sleep(3);
    printf("Slot Successfully Booked in %s\n\n", hosp_name);
    printf("Visit The Hospital In Next 48 hours\n\n");
    fclose(fp1);
    return;
}

void pin(char *aadhar, char *pin_code)
{
    system("CLS");
    char pin_code_para[50];
    strcpy(pin_code_para, pin_code);
    strcat(pin_code_para, ".txt");
    int choice;
    char hosp_name[50], decision, aadhar_compare[13];
    FILE *fp, *fp1;
    fp = fopen(pin_code_para, "r+");
    fp1 = fopen("vac_book.txt", "a+");
    if (fp == NULL || fp1 == NULL)
    {
        printf("*****ERROR IN SYSTEM!!!TRY LATER*****");
        return;
    }
hosp_display:
    system("CLS");
    int i = 1;
    rewind(fp);
    printf("Available Hospitals Are:\n\n");
    while (!feof(fp))
    {
        fscanf(fp, " %[^\n]s", hosp_name);
        printf("%d.%s\n\n", i, hosp_name);
        i++;
    }
    printf("Enter your Choice: ");
    scanf("%d", &choice);
    printf("\n\n");
    rewind(fp);
    for (int i = 0; i < choice; i++)
    {
        fscanf(fp, " %[^\n]s", hosp_name);
    }
    hosp_book(aadhar, hosp_name, pin_code);
    if (check == 1)
    {
        goto hosp_display;
    }
    fclose(fp);
    fclose(fp1);
}

void vac_slot_booking()
{
    FILE *fp, *fp1;
    fp = fopen("dose_log.txt", "r+");
    fp1 = fopen("vaccination_book.txt", "a+");
    int choice, dose1, dose2;
    char hosp_name[50], aadhar[13], aadhar_compare[13], vac_type[15], pin_code[8];
    printf("\t\t\t\t\t\t\tVACCINATION SLOT BOOKING\n\n");
aadhar_input:
    rewind(fp);
    rewind(fp1);
    printf(" Enter The Registered Aadhar Number: ");
    scanf("%s", aadhar);
    while (!feof(fp1))
    {
        fscanf(fp1, "%s", aadhar_compare);
        if (!strcmp(aadhar_compare, aadhar))
        {
            printf("\n\n\tAadhar Already Registered for 1st dose\n\n");
            sleep(2);
            system("cls");
            goto aadhar_input;
        }
    }
    while (!feof(fp))
    {
        fscanf(fp, "%s", aadhar_compare);
        if (!strcmp(aadhar_compare, aadhar))
        {
            fscanf(fp, "%d", &dose1);
            fscanf(fp, "%d", &dose2);
            if (dose1 == 1 && dose2 == 1)
            {
                printf("\n\nYou Are Fully Vaccinated\n\n");
                sleep(3);
                return;
            }
            else if (dose1 == 1 && dose2 == 0)
            {
                fscanf(fp, "%s", vac_type);
                fscanf(fp, " %[^\n]s", hosp_name);
                printf("\n\nYour 1st Dose is done,Your 2nd Dose of %s Is Booked In %s\n\n", vac_type, hosp_name);
                sleep(3);
                return;
            }
        }
    }
    fclose(fp);
    compare(aadhar);
    if (Aadhar_flag == 0)
        goto pin_display;
    else if (Aadhar_flag == 1)
    {
        system("cls");
        goto aadhar_input;
    }
pin_display:
    printf("Choose One of The Centers\n");
    printf("1.560061\n\n2.560069\n\n3.560073\n\n4.560043\n\n5.560083\n\n6.560100\n\n");
    printf("\n\nEnter your Choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        strcpy(pin_code, "560061");
        pin(aadhar, pin_code);
        break;
    case 2:
        strcpy(pin_code, "560069");
        pin(aadhar, pin_code);
        break;
    case 3:
        strcpy(pin_code, "560073");
        pin(aadhar, pin_code);
        break;
    case 4:
        strcpy(pin_code, "560043");
        pin(aadhar, pin_code);
        break;
    case 5:
        strcpy(pin_code, "560083");
        pin(aadhar, pin_code);
        break;
    case 6:
        strcpy(pin_code, "560100");
        pin(aadhar, pin_code);
        break;
    }
}

FILE *file_open(char *file_name)
{
    FILE *fp = fopen(file_name, "a+");
    return fp;
}

void sec_key_gen(FILE *fp)
{
    int i;
    char secret_key[8];
    srand(time(0));
    for (i = 0; i < 6; i++)
        secret_key[i] = keys[rand() % (61)]; // 62 is the string length of the array keys, (62-1) is done in order to produce indexes from 0 to 61
    secret_key[7] = '\0';
    fprintf(fp, "%s\n", secret_key);
}

int checks(char *s)
{
    char s_cmp[13];
    FILE *f = fopen("reg_log.txt", "r");
    while (!feof(f))
    {
        fscanf(f, "%s", s_cmp);
        if (!strcmp(s, s_cmp))
            return 1;
    }
    return 0;
}

void registration(char *ph_no)
{
    system("cls");
    FILE *fp = file_open("reg_log.txt");
    int i, numOfPpl, choice;
    char adh_no[13], one_char, ph_no_cmp[11], secretKey[8], name[40];

    while (!feof(fp))
    {
        fscanf(fp, "%s", ph_no_cmp);
        if (strlen(ph_no_cmp) == 10 && strcmp(ph_no, ph_no_cmp) == 0)
        {
            printf("You have already registered\n");
            rewind(fp);
            return;
        }
    }

    printf("Registration can be performed only once so please enter aadhar numbers carefully\n\n");
    rewind(fp);
no_of_ppl:
    printf("***NOTE: Only 4 people can register per number***\n\n");
    printf("Enter number of people you want to register for vaccination: ");
    scanf("%d", &numOfPpl);
    if (numOfPpl > 4)
    {
        printf("\n***Please adhere to the norms***\n\n");
        redirecting("in");
        goto no_of_ppl;
    }
    // updating the information in the register_log file
    fprintf(fp, "%s\n", ph_no);
    for (i = 0; i < numOfPpl; i++)
    {
    ent_adh:
        printf("\n");
        printf("Enter patient name: ");
        scanf(" %[^\n]s", name);
        strupr(name);
        fprintf(fp, "%s\n", name);
        printf("\n");
        printf("Please enter Aadhar number: ");
        scanf("%s", adh_no);

        if (adh_no[0] == 1 || adh_no[0] == 0)
        {
            printf("Aadhar number can not start with 0 or 1\n");
            goto ent_adh;
        }

        if (checks(adh_no))
        {
            printf("\n\nAadhar number is already registered through another phone number\n\n");
            goto ent_adh;
        }
        printf("\n");
        if (strlen(adh_no) != 12)
        {
            printf("Aadhar number must be of 12 digits\n\n");
            goto ent_adh;
        }
        fprintf(fp, "%s\n", adh_no);
        sec_key_gen(fp);
    }
    printf("\n\nREGISTRATION COMPLETED\n\n");
    redirecting("to main menu in");
    fclose(fp);
    return;
}

//***********************************************HELP AND FEEDBACK*****************************************************

void help_feedback()
{
    int ch;
    while (1)
    {
        system("CLS");
        printf("****COVID-19 Customer Support****\n\n");
        printf("1.Help\n2.Feedback\nPress Any Other Key To Exit\n");
        printf("Enter Your Choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            help();
            break;
        case 2:
            feedback();
            break;
        default:
            return;
        }
    }
}

void help()
{
    system("CLS");
    printf("****COVID-19 Customer Support****\n\n");
    printf("Contact Number: **********\n                **********\n");
    printf("\nEmail: **********\n");
    sleep(3);
    return;
}

void feedback()
{
    system("START https://forms.gle/rHYs1v4eetusxcnU7");
    return;
}
