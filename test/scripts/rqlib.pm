#
# Copyright (c) 2002 - Brett Hutley <brett@hutley.net>
#
# This file is part of Risk Quantify
# distributed under the LGPL
#
require librqperl;

use strict;

sub load_system
{
	my $data_dir = shift;

	my $rqsys = librqperl::rq_system_alloc();

	load_currencies($rqsys, $data_dir);
	load_currency_pairs($rqsys, $data_dir);
	load_irbonds($rqsys, $data_dir);
	load_irovernights($rqsys, $data_dir);
	load_irdiscounts($rqsys, $data_dir);

	return $rqsys;
}

sub load_currencies
{
	my $rqsys = shift;
	my $data_dir = shift;

	open(CCYS, "< $data_dir/currencies.txt") or die "Couldn't open the $data_dir/currencies.txt file\n";

	while (<CCYS>)
	{
		chomp;
		my @vals = split(/\|/);
		if (scalar(@vals) == 3)
		{
			my $accy = librqperl::rq_asset_ccy_build($vals[0], $vals[1], $vals[2]);
			librqperl::rq_asset_mgr_add(librqperl::rq_system_get_asset_mgr($rqsys), $accy);
		}
	}

	close(CCYS);
	
}


sub load_currency_pairs
{
	my $rqsys = shift;
	my $data_dir = shift;

	open(CCYPAIRS, "< $data_dir/currency_pairs.txt") or die "Couldn't open the $data_dir/currency_pairs.txt file\n";

	while (<CCYPAIRS>)
	{
		chomp;
		my @vals = split(/\|/);
		if (scalar(@vals) == 5)
		{
			my $one_per_two = 1;
			if ($vals[3] eq '2/1')
			{
				$one_per_two = 2;
			}
			my $accy = librqperl::rq_asset_ccypair_build($vals[0], $vals[1], $vals[2], $one_per_two, $vals[4]);
			librqperl::rq_asset_mgr_add(librqperl::rq_system_get_asset_mgr($rqsys), $accy);
		}
	}
	
	close(CCYPAIRS);
}

sub load_irbonds
{
	my $rqsys = shift;
	my $data_dir = shift;

	my @asset_bond_filenames = (
								"$data_dir/asset_irbonds.txt",
								);

	for (my $i = 0; $i < scalar(@asset_bond_filenames); $i++)
	{
		my $asset_bond_filename = $asset_bond_filenames[$i];

		open(BNDS, "< $asset_bond_filename") or die "Couldn't open $asset_bond_filename";
		while (<BNDS>)
		{
			chomp;
			if (substr($_, 0, 1) ne "#")
			{
				my @flds = split(/\|/);
				if (scalar(@flds) == 6)
				{
					my $mat_date = librqperl::rq_date_parse($flds[2], 3);
					my @t = split(/,/, $flds[3]);
					my $term = librqperl::rq_term_alloc();
					librqperl::rq_term_fill($term, $t[3], $t[2], $t[1], $t[0]);
					my $asset = librqperl::rq_asset_irbond_build(
																 $flds[0], 
																 $flds[1],
																 $mat_date,
																 $term,
																 2,
																 2
																 );
					librqperl::rq_asset_mgr_add(librqperl::rq_system_get_asset_mgr($rqsys),
												$asset
												);
				}
			}
		}
	}
	
	close(BNDS);
}

sub load_irovernights
{
	my $rqsys = shift;
	my $data_dir = shift;

	my $asset_overnight_filename = "$data_dir/asset_overnight.txt";

	open(ON, "< $asset_overnight_filename") or die "Couldn't open $asset_overnight_filename";
	while (<ON>)
	{
		chomp;
		if (substr($_, 0, 1) ne "#")
		{
			my @flds = split(/\|/);
			if (scalar(@flds) == 3)
			{
				my $day_count = 2;
				if ($flds[2] eq "360")
				{
					$day_count = 1;
				}
				my $asset = librqperl::rq_asset_irovernight_build(
																  $flds[0], 
																  $flds[1],
																  $day_count
																 );
				librqperl::rq_asset_mgr_add(librqperl::rq_system_get_asset_mgr($rqsys),
											$asset
											);
			}
		}
	}
	
	close(ON);
}

sub load_irdiscounts
{
	my $rqsys = shift;
	my $data_dir = shift;

	my $asset_discount_filename = "$data_dir/asset_irdiscounts.txt";

	open(DISC, "< $asset_discount_filename") or die "Couldn't open $asset_discount_filename";
	while (<DISC>)
	{
		chomp;
		if (substr($_, 0, 1) ne "#")
		{
			my @flds = split(/\|/);
			if (scalar(@flds) == 4)
			{
                my ( $years, $months, $weeks, $days ) = split(/,/, $flds[2]);
				my $day_count = 2;
				if ($flds[3] eq "360")
				{
					$day_count = 1;
				}
                my $tenor = librqperl::rq_term_build($days, $weeks, $months, $years);
				my $asset = librqperl::rq_asset_irdiscount_build(
                                                                 $flds[0], 
                                                                 $flds[1],
                                                                 $tenor,
                                                                 $day_count
																 );
                librqperl::rq_term_free($tenor);
				librqperl::rq_asset_mgr_add(librqperl::rq_system_get_asset_mgr($rqsys),
											$asset
											);
			}
		}
	}
	
	close(DISC);
}

sub load_rates
{
	my $market = shift;
	my $data_dir = shift;

	my $rates_filename = "$data_dir/rates.txt";

	open(R, "< $rates_filename") or die "Couldn't open $rates_filename";
	while (<R>)
	{
		chomp;
		if (substr($_, 0, 1) ne "#")
		{
			my @flds = split(/\|/);
			if (scalar(@flds) >= 6)
			{
				my $obs_date = librqperl::rq_date_parse($flds[3], 3);
				if (librqperl::rq_date_diff($obs_date, librqperl::rq_market_get_market_date($market)) == 0)
				{
					my $rate_type = 1;
					if ($flds[2] eq 'SIMPLE')
					{
						$rate_type = 1;
					}
					elsif ($flds[2] eq 'COMPOUNDING')
					{
						$rate_type = 2;
					}
					elsif ($flds[2] eq 'PAR')
					{
						$rate_type = 3;
					}
					elsif ($flds[2] eq 'EXCHANGE')
					{
						$rate_type = 4;
					}
					elsif ($flds[2] eq 'VOLATILITY')
					{
						$rate_type = 5;
					}
					my $val_date = librqperl::rq_date_parse($flds[4], 3);
					my $rate = librqperl::rq_rate_build(
														$flds[1], 
														$flds[1],
														$rate_type,
														$obs_date,
														$val_date,
														$flds[5]
														);
					librqperl::rq_rate_mgr_add(librqperl::rq_market_get_rate_mgr($market),
													$rate
													);
				}
			}
		}
	}
	
	close(R);
}
