/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:37:34 by rasamad           #+#    #+#             */
/*   Updated: 2024/03/07 14:29:12 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_init(int ac, t_var *var, char **av)
{
	if (ac != 5)
	{
		ft_printf("Error\ninvalid argument\n");
		exit(0);
	}
	var->infile = av[1];
	var->cmd1 = av[2];
	var->cmd2 = av[3];
	var->outfile = av[4];
	var->check_access_cmd1 = 0;
	var->check_access_cmd2 = 0;
	var->path_cmd1 = NULL;
	var->path_cmd2 = NULL;
	var->open_in = 0;
	var->open_out = 0;
}

void	ft_open_fd(t_var *var)
{
	var->fd_infile = open(var->infile, O_RDONLY);
	if (var->fd_infile == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		write(2, ": ", 2);
		ft_putstr_fd(var->infile, 2);
		write(2, "\n", 1);
		var->open_in = -1;
	}
	var->fd_outfile = open(var->outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (var->fd_outfile == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		write(2, ": ", 2);
		ft_putstr_fd(var->outfile, 2);
		write(2, "\n", 1);
		var->open_out = -1;
	}
}

void	ft_close_fd(t_var *var)
{
	if (var->fd_infile != -1)
	{
		close(var->fd_infile);
		var->fd_infile = -1;
	}
	if (var->pipe_fd[1] != -1)
	{
		close(var->pipe_fd[1]);
		var->pipe_fd[1] = -1;
	}
	if (var->pipe_fd[0] != -1)
	{
		close(var->pipe_fd[0]);
		var->pipe_fd[0] = -1;
	}
	if (var->fd_outfile != -1)
	{
		close(var->fd_outfile);
		var->fd_outfile = -1;
	}
}

void	ft_free(t_var *var, int status)
{
	int	i;

	i = 0;
	while (var->split_path[i] != NULL)
	{
		free(var->split_path[i]);
		i++;
	}
	free(var->split_path);
	if (status == 1)
		free(var->cmd);
}

void	ft_free_split_flag(t_var *var)
{
	int	i;

	i = 0;
	while (var->split_flag[i] != NULL)
	{
		free(var->split_flag[i]);
		i++;
	}
	free(var->split_flag);
}

/*

#include "Account.hpp"
#include <iostream>
#include <ctime>

// Initialisation des variables statiques
int Account::_nbAccounts = 0;
int Account::_totalAmount = 0;
int Account::_totalNbDeposits = 0;
int Account::_totalNbWithdrawals = 0;

// Constructeur
Account::Account(int initial_deposit) : 
    _accountIndex(_nbAccounts), 
    _amount(initial_deposit), 
    _nbDeposits(0), 
    _nbWithdrawals(0) 
{
    _nbAccounts++;
    _totalAmount += initial_deposit;
    _displayTimestamp();
    std::cout << "index:" << _accountIndex << ";amount:" << _amount << ";created" << std::endl;
}

// Destructeur
Account::~Account(void) {
    _displayTimestamp();
    std::cout << "index:" << _accountIndex << ";amount:" << _amount << ";closed" << std::endl;
    _nbAccounts--;
    _totalAmount -= _amount;
}

// Méthodes statiques
int Account::getNbAccounts(void) {
    return _nbAccounts;
}

int Account::getTotalAmount(void) {
    return _totalAmount;
}

int Account::getNbDeposits(void) {
    return _totalNbDeposits;
}

int Account::getNbWithdrawals(void) {
    return _totalNbWithdrawals;
}

void Account::displayAccountsInfos(void) {
    _displayTimestamp();
    std::cout << "accounts:" << getNbAccounts() << ";total:" << getTotalAmount() 
              << ";deposits:" << getNbDeposits() << ";withdrawals:" << getNbWithdrawals() 
              << std::endl;
}

// Méthode de dépôt
void Account::makeDeposit(int deposit) {
    _displayTimestamp();
    _amount += deposit;
    _nbDeposits++;
    _totalNbDeposits++;
    _totalAmount += deposit;
    std::cout << "index:" << _accountIndex << ";p_amount:" << _amount - deposit
              << ";deposit:" << deposit << ";amount:" << _amount << ";nb_deposits:" << _nbDeposits << std::endl;
}

// Méthode de retrait
bool Account::makeWithdrawal(int withdrawal) {
    _displayTimestamp();
    if (withdrawal > _amount) {
        std::cout << "index:" << _accountIndex << ";p_amount:" << _amount 
                  << ";withdrawal:refused" << std::endl;
        return false;
    }
    _amount -= withdrawal;
    _nbWithdrawals++;
    _totalNbWithdrawals++;
    _totalAmount -= withdrawal;
    std::cout << "index:" << _accountIndex << ";p_amount:" << _amount + withdrawal 
              << ";withdrawal:" << withdrawal << ";amount:" << _amount 
              << ";nb_withdrawals:" << _nbWithdrawals << std::endl;
    return true;
}

// Vérification du montant du compte
int Account::checkAmount(void) const {
    return _amount;
}

// Affichage du statut du compte
void Account::displayStatus(void) const {
    _displayTimestamp();
    std::cout << "index:" << _accountIndex << ";amount:" << _amount 
              << ";deposits:" << _nbDeposits << ";withdrawals:" << _nbWithdrawals << std::endl;
}

// Méthode d'affichage de l'horodatage
void Account::_displayTimestamp(void) {
    std::time_t now = std::time(0);
    std::tm *ltm = std::localtime(&now);
    std::cout << "[" << 1900 + ltm->tm_year 
              << (ltm->tm_mon < 9 ? "0" : "") << 1 + ltm->tm_mon 
              << (ltm->tm_mday < 10 ? "0" : "") << ltm->tm_mday 
              << "_" << (ltm->tm_hour < 10 ? "0" : "") << ltm->tm_hour 
              << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min 
              << (ltm->tm_sec < 10 ? "0" : "") << ltm->tm_sec << "] ";
}

*/
