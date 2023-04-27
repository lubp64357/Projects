<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/bug-tracking">
    <project-members>
      <xsl:apply-templates select="project/defect/owner"/>
    </project-members>
  </xsl:template>
  <xsl:template match="owner">
    <member>
      <xsl:attribute name="project">
        <xsl:value-of select="../../../project/@name"/>
      </xsl:attribute>
      <xsl:text><xsl:value-of select="substring-after(.,' ')"/></xsl:text>
      <xsl:text>, </xsl:text>
      <xsl:text><xsl:value-of select="substring-before(.,' ')"/></xsl:text>	    
    </member>
  </xsl:template>
</xsl:stylesheet>