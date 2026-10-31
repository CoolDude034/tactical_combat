function PlayerRunCommand()
{
	if (self.IsNoclipping() == false && ::m_bSlowWalker)
	{
		command.SetForwardMove(command.GetForwardMove() * 0.1)
		command.SetSideMove(command.GetSideMove() * 0.1)
		command.SetUpMove(command.GetUpMove() * 0.1)
	}
}