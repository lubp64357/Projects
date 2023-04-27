package junit5_basics;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.*;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Nested;
import org.junit.jupiter.api.RepeatedTest;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInfo;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestReporter;
import org.junit.jupiter.api.condition.EnabledOnOs;
import org.junit.jupiter.api.condition.OS;

@DisplayName("When running MathUtils")
class MathUtilsTest {
	MathUtils mathUtils;
	TestInfo testInfo; 
	TestReporter testReporter;
	
	@BeforeEach
	void init(TestInfo testInfo, TestReporter testReporter) {
		this.testInfo = testInfo;
		this.testReporter = testReporter;
		mathUtils = new MathUtils();
		testReporter.publishEntry("Runnig "+ testInfo.getDisplayName() + " with tags " + testInfo.getTags());
	}
	
	
	@Nested
	@DisplayName("Add Method")
	@Tag("Math")
	class AddTest {
		@Test
		@DisplayName("When adding 2 positive nums")
		void testAddPositive() {
			assertEquals(2, mathUtils.add(1, 1), "should return the right sum!");

		}

		@Test
		@DisplayName("When adding 2 negative nums")
		void testAddNegative() {
			int expected = 2;
			int actual = mathUtils.add(1, 1);
			assertEquals(expected, actual, () -> "should return " + expected + " but returned " + actual);

		}
	}
	
	
	
	@Test
	@DisplayName("Test /0 ")
	@Tag("Math")
	void testDevide() {
		boolean isServerUp = true;
		assumeTrue(isServerUp);
		assertThrows(ArithmeticException.class, () -> mathUtils.divide(1,0), "Exception came!");
		}
	
	@RepeatedTest(3)
	@Tag("Circle")
	void testComputeCircleRadius() {
		assertEquals(Math.PI, mathUtils.computeCircleArea(1), "Should be true");
	}
	

	
	@Test
	@DisplayName("Multiply method")
	@Tag("Math")
	void testMultiply() {
		
		assertAll(
				() -> assertEquals(4, mathUtils.multiply(2, 2)),
				() -> assertEquals(0, mathUtils.multiply(0, 2)),
				() -> assertEquals(-2, mathUtils.multiply(2, -1))
				);
	}
	
	
	
}
